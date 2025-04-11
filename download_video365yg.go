// @author      Liu Yongshuai<liuyongshuai@hotmail.com>
// @file        download_video365yg.go
// @date        2025-04-11 11:02

package main

import (
	"context"
	//"encoding/base64"
	"encoding/json"
	"fmt"
	"github.com/liuyongshuai/negoutils"
	"hash/crc32"
	"strconv"
	"strings"
	"time"
)

type TouTiaoVideoInfo struct {
	Definition string `json:"definition"`
	Quality    string `json:"quality"`
	VType      string `json:"vtype"`
	VWidth     int    `json:"vwidth"`
	VHeight    int    `json:"vheight"`
	CodecType  string `json:"codec_type"`
	Size       int64  `json:"size"`
	MainURL    string `json:"main_url"`
	FileId     string `json:"file_id"`
}

type TouTiaoVideo365YGInfo struct {
	Code    int64  `json:"code"`
	Total   int64  `json:"total"`
	Message string `json:"message"`
	Data    struct {
		Status         int                         `json:"status"`
		Message        string                      `json:"message"`
		EnableSSL      bool                        `json:"enable_ssl"`
		AutoDefinition string                      `json:"auto_definition"`
		EnableAdaptive bool                        `json:"enable_adaptive"`
		VideoId        string                      `json:"video_id"`
		VideoDuration  float64                     `json:"video_duration"`
		MediaType      string                      `json:"media_type"`
		VideoList      map[string]TouTiaoVideoInfo `json:"video_list"`
		UrlExpire      int                         `json:"url_expire"`
		PostUrl        string                      `json:"post_url"`
	} `json:"data"`
}

// 从阳光宽频网上抓取下来的视频信息
type Video365ygInfo struct {
	AutoID  uint64 `json:"-" db:"auto_id"`         //无意义的自增ID
	VideoID string `json:"video_id" db:"video_id"` //视频ID
}

var (
	db             *negoutils.DBase
	myconf         negoutils.MySQLConf
	video_url      = "http://ib.365yg.com/video/urls/v/1/toutiao/mp4/"
	videoSelectSQL = "SELECT `auto_id`,`video_id` FROM `video_365yg` WHERE `auto_id` > ? AND `video_size` = 0 AND `is_forbidden` = 0 ORDER BY `auto_id` ASC LIMIT 100"
	videoUpdateSQL = "UPDATE `video_365yg` SET `video_size` = ? WHERE `auto_id` = ?"
	videoForbidSQL = "UPDATE `video_365yg` SET `is_forbidden` = 1 WHERE `auto_id` = ?"
	videoChan      = make(chan Video365ygInfo, 10000)
)

func main() {
	myconf = negoutils.MySQLConf{
		Charset:      "utf8",
		Host:         "127.0.0.1",
		User:         "phpmyadmin",
		Passwd:       "123456",
		DbName:       "db_scrapy",
		Timeout:      10,
		Port:         3306,
		AutoCommit:   true,
		MaxIdleConns: 10,
	}
	db = negoutils.NewDBase(myconf)
	_, err := db.Conn()
	if err != nil {
		fmt.Println(err)
		return
	}
	defer db.Close()

	//多goroutine处理
	for i := 0; i < 24; i++ {
		go downloadVideo()
	}

	videoAutoId := uint64(0)
	for {
		fmt.Println("videoAutoId=", videoAutoId)
		rows, err := db.FetchRows(videoSelectSQL, videoAutoId)
		if err != nil {
			fmt.Println(err)
			return
		}
		if len(rows) == 0 {
			break
		}

		for _, row := range rows {
			var videoInfo Video365ygInfo
			videoInfo.AutoID, _ = row["auto_id"].ToUint64()
			videoInfo.VideoID = row["video_id"].ToString()
			videoAutoId = videoInfo.AutoID
			videoChan <- videoInfo
		}
	}
	close(videoChan)
}

func downloadVideo() {
	vid, ok := <-videoChan
	if !ok {
		return
	}
	auto_id := vid.AutoID
	video_id := vid.VideoID
	t := time.Now().UnixNano() / 1000000
	callback := "reqwest_" + strconv.FormatUint(uint64(t), 10)
	url := video_url + video_id
	um, _ := negoutils.ParseUrl(url, -1)
	path := um["path"]
	s := crc32.ChecksumIEEE([]byte(path + "?r=" + strconv.FormatUint(uint64(t), 10)))
	url += "?r=" + strconv.FormatUint(uint64(t), 10) + "&s=" + strconv.FormatUint(uint64(s), 10) + "&callback=" + callback
	//fmt.Println(url)
	//fmt.Println("\n\n")

	client := negoutils.NewHttpClient(url, context.Background())
	client.SetReferer("http://365yg.com")
	client.AddHeader("myHeaderKey", "myHeaderValue")
	client.SetUserAgent("Mozilla/5.0 (Linux; Android 6.0.1; SM919 Build/MXB48T; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/55.0.2883.84 Mobile Safari/537.36 JsSdk/2 NewsArticle/6.2.7 NetType/wifi")
	client.SetKeepAlive(false)
	resp, e := client.Get()
	if e != nil {
		fmt.Println("get url failed", e)
		return
	}
	ret := resp.GetBodyString()
	ret = strings.TrimLeft(ret, callback)
	ret = strings.TrimLeft(ret, "(")
	ret = strings.TrimRight(ret, ")")
	//fmt.Println(ret)
	//fmt.Println("\n\n")
	vinfo := TouTiaoVideo365YGInfo{}
	e = json.Unmarshal([]byte(ret), &vinfo)
	if e != nil {
		fmt.Println("json.Unmarshal vinfo failed", e)
		return
	}

	//是否被平台封禁
	if vinfo.Code != 0 {
		_, _, e = db.Execute(videoForbidSQL, auto_id)
		fmt.Println("FORBIDDEN", e, " video_id=", video_id, "message=", vinfo.Message, ", auto_id=", auto_id)
		return
	}

	//fmt.Println(vinfo)
	//fmt.Println("\n\n")
	//tmpDecode, _ := base64.StdEncoding.DecodeString(vinfo.Data.PostUrl)
	//vinfo.Data.PostUrl = negoutils.ByteToStr(tmpDecode)
	vlist := vinfo.Data.VideoList
	//mainUrl := ""
	vSize := int64(0)
	for _, v := range vlist {
		//tmpDecode, e := base64.StdEncoding.DecodeString(v.MainURL)
		//if e != nil {
		//	continue
		//}
		//mainUrl = negoutils.ByteToStr(tmpDecode)
		if v.Size > 0 {
			vSize = v.Size
			break
		}
		//fmt.Println("mainUrl", mainUrl)
		//fmt.Println("videoSize", v.Size)
	}
	if vSize == 0 {
		fmt.Println("invalid vSize")
		//fmt.Println(vinfo)
		return
	}
	_, _, e = db.Execute(videoUpdateSQL, vSize, auto_id)
	if e != nil {
		fmt.Println("update VideoSize failed", e)
		return
	}
}
