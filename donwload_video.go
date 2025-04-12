// @author      Liu Yongshuai<liuyongshuai@hotmail.com>
// @file        donwload_video.go
// @date        2025-04-12 11:46

package main

import (
	"bufio"
	"context"
	"encoding/base64"
	"encoding/json"
	"flag"
	"fmt"
	"github.com/liuyongshuai/negoutils"
	"hash/crc32"
	"strconv"
	"strings"
	"time"
)

var (
	video_id   string
	db         *negoutils.DBase
	myconf     negoutils.MySQLConf
	video_url  = "http://ib.365yg.com/video/urls/v/1/toutiao/mp4/"
	videoSQL   = "SELECT * FROM `video_365yg` WHERE `video_id` = ? AND `video_size` > 0 ORDER BY `comment_num` DESC LIMIT 1"
	videoUPSQL = "UPDATE `video_365yg` SET `is_publish`=1 WHERE `video_id` = ?"
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
		PosterUrl      string                      `json:"poster_url"`
	} `json:"data"`
}

// ./donwload_video --video_id 4454f562ba224cad8d141c6e2fa75834
func main() {
	flag.StringVar(&video_id, "video_id", "", "The greeting object.")
	flag.Parse()
	if video_id == "" {
		fmt.Println("invalid video id")
		return
	}
	myconf = negoutils.MySQLConf{
		Charset:      "utf8",
		Host:         "10.40.28.178",
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

	row, err := db.FetchRow(videoSQL, video_id)
	if err != nil {
		fmt.Println(err)
		return
	}

	//格式化视频信息
	auto_id, _ := row["auto_id"].ToUint64()
	title := row["title"].ToString()
	video_desc := row["video_desc"].ToString()
	t := time.Now().UnixNano() / 1000000
	callback := "reqwest_" + strconv.FormatUint(uint64(t), 10)
	url := video_url + video_id
	um, _ := negoutils.ParseUrl(url, -1)
	path := um["path"]
	s := crc32.ChecksumIEEE([]byte(path + "?r=" + strconv.FormatUint(uint64(t), 10)))
	url += "?r=" + strconv.FormatUint(uint64(t), 10) + "&s=" + strconv.FormatUint(uint64(s), 10) + "&callback=" + callback
	tmpByte, e := download(url)
	if e != nil {
		fmt.Println(e)
		return
	}
	ret := negoutils.ByteToStr(tmpByte)
	ret = strings.TrimLeft(ret, callback)
	ret = strings.TrimLeft(ret, "(")
	ret = strings.TrimRight(ret, ")")
	vinfo := TouTiaoVideo365YGInfo{}
	e = json.Unmarshal([]byte(ret), &vinfo)
	if e != nil {
		fmt.Println("json.Unmarshal vinfo failed", e)
		return
	}

	if vinfo.Code != 0 {
		fmt.Println(vinfo.Message)
		return
	}

	post_url := vinfo.Data.PosterUrl
	vlist := vinfo.Data.VideoList
	mainUrl := ""
	for _, v := range vlist {
		tmpDecode, e := base64.StdEncoding.DecodeString(v.MainURL)
		if e != nil {
			continue
		}
		mainUrl = negoutils.ByteToStr(tmpDecode)
		break
	}
	fmt.Println("auto_id：", auto_id)
	fmt.Println("video_id：", video_id)
	fmt.Println("title：", title)
	fmt.Println("video_desc：", video_desc)
	fmt.Println("post_url：", post_url)
	fmt.Println("video_url：", mainUrl)

	//下载
	picStr, e := download(post_url)
	if e != nil {
		fmt.Println(e)
		return
	}
	picurl := video_id + ".jpg"
	fp, e := negoutils.OpenNewFile(picurl, "", false)
	if e != nil {
		fmt.Println(e)
		return
	}
	writer := bufio.NewWriter(fp)
	writer.Write(picStr)
	writer.Flush()
	fp.Close()
	videoStr, e := download(mainUrl)
	if e != nil {
		fmt.Println(e)
		return
	}
	videourl := video_id + ".mp4"
	fp, e = negoutils.OpenNewFile(videourl, "", false)
	if e != nil {
		fmt.Println(e)
		return
	}
	writer = bufio.NewWriter(fp)
	writer.Write(videoStr)
	writer.Flush()
	fp.Close()
	if negoutils.FileExists(videourl) && negoutils.FileExists(picurl) {
		db.Execute(videoUPSQL, video_id)
	}
	return
}

func download(url string) ([]byte, error) {
	client := negoutils.NewHttpClient(url, context.Background())
	client.SetReferer("http://365yg.com")
	client.AddHeader("myHeaderKey", "myHeaderValue")
	client.SetTimeout(100 * time.Second)
	client.SetUserAgent("Mozilla/5.0 (Linux; Android 6.0.1; SM919 Build/MXB48T; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/55.0.2883.84 Mobile Safari/537.36 JsSdk/2 NewsArticle/6.2.7 NetType/wifi")
	client.SetKeepAlive(false)
	resp, e := client.Get()
	if e != nil {
		fmt.Println("get url failed", e)
		return nil, e
	}
	return resp.GetBody(), nil
}
