// @author      Liu Yongshuai<liuyongshuai@hotmail.com>
// @file        download_video365yg.go
// @date        2025-04-11 11:02

package main

import (
	"context"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"github.com/liuyongshuai/negoutils"
	"hash/crc32"
	"strconv"
	"strings"
	"time"
)

type VideoInfo struct {
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

type Video365YGInfo struct {
	Code    int64  `json:"code"`
	Total   int64  `json:"total"`
	Message string `json:"message"`
	Data    struct {
		Status         int                  `json:"status"`
		Message        string               `json:"message"`
		EnableSSL      bool                 `json:"enable_ssl"`
		AutoDefinition string               `json:"auto_definition"`
		EnableAdaptive bool                 `json:"enable_adaptive"`
		VideoId        string               `json:"video_id"`
		VideoDuration  float64              `json:"video_duration"`
		MediaType      string               `json:"media_type"`
		VideoList      map[string]VideoInfo `json:"video_list"`
		UrlExpire      int                  `json:"url_expire"`
		PostUrl        string               `json:"post_url"`
	} `json:"data"`
}

func main() {
	t := time.Now().UnixNano()
	video_id := "5494cda9784e4f60b04e4c0c6629da56"
	video_url := "http://ib.365yg.com/video/urls/v/1/toutiao/mp4/"
	callback := "reqwest_" + strconv.FormatUint(uint64(t), 10)
	url := video_url + video_id
	um, _ := negoutils.ParseUrl(url, -1)
	path := um["path"]
	s := crc32.ChecksumIEEE([]byte(path + "?r=" + strconv.FormatUint(uint64(t), 10)))
	url += "?r=" + strconv.FormatUint(uint64(t), 10) + "&s=" + strconv.FormatUint(uint64(s), 10) + "&callback=" + callback
	fmt.Println(url)
	fmt.Println("\n\n")

	client := negoutils.NewHttpClient(url, context.Background())
	client.SetReferer("http://365yg.com.com")
	client.AddHeader("myHeaderKey", "myHeaderValue")
	client.SetUserAgent("Mozilla/5.0 (Linux; Android 6.0.1; SM919 Build/MXB48T; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/55.0.2883.84 Mobile Safari/537.36 JsSdk/2 NewsArticle/6.2.7 NetType/wifi")
	client.SetKeepAlive(true)
	resp, _ := client.Get()
	ret := resp.GetBodyString()
	ret = strings.TrimLeft(ret, callback)
	ret = strings.TrimLeft(ret, "(")
	ret = strings.TrimRight(ret, ")")
	fmt.Println(ret)
	fmt.Println("\n\n")
	vinfo := Video365YGInfo{}
	json.Unmarshal([]byte(ret), &vinfo)
	fmt.Println(vinfo)
	fmt.Println("\n\n")
	//tmpDecode, _ := base64.StdEncoding.DecodeString(vinfo.Data.PostUrl)
	//vinfo.Data.PostUrl = negoutils.ByteToStr(tmpDecode)
	vlist := vinfo.Data.VideoList
	mainUrl := ""
	for _, v := range vlist {
		tmpDecode, e := base64.StdEncoding.DecodeString(v.MainURL)
		if e != nil {
			continue
		}
		mainUrl = negoutils.ByteToStr(tmpDecode)
		fmt.Println("mainUrl", mainUrl)
		fmt.Println("videoSize", v.Size)
		break
	}

}
