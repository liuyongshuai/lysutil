// @author      Liu Yongshuai<liuyongshuai@hotmail.com>
// @file        dump_scrapy.go
// @date        2025-04-02 09:58

package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"github.com/liuyongshuai/negoutils"
)

var (
	db     *negoutils.DBase
	myconf negoutils.MySQLConf
	sfile  string
)

// 帖子
type Article struct {
	AutoID  uint64 `json:"-" db:"auto_id"`       //无意义的自增ID
	URL     string `json:"url" db:"url"`         //url
	Title   string `json:"title" db:"title"`     //标题
	Content string `json:"content" db:"content"` //内容
	Tags    string `json:"tags" db:"tags"`       //标签
}

// 段子信息
type JokeText struct {
	AutoID  uint64 `json:"-" db:"auto_id"`       //无意义的自增ID
	Md5     string `json:"md5" db:"md5"`         //段子的ID
	Content string `json:"content" db:"content"` //内容
}

// 从阳光宽频网上抓取下来的视频信息
type Video365yg struct {
	AutoID        uint64              `json:"-" db:"auto_id"`                     //无意义的自增ID
	GroupID       uint64              `json:"group_id" db:"group_id"`             //http://www.365yg.com/group/{group_id}/组成视频详情页
	Title         string              `json:"title" db:"title"`                   //标题
	VideoDuration uint                `json:"video_duration" db:"video_duration"` //视频时长
	VideoID       string              `json:"video_id" db:"video_id"`             //视频ID
	VideoDesc     string              `json:"video_desc" db:"video_desc"`         //视频的描述信息
	CommentNum    uint                `json:"comment_num" db:"comment_num"`       //评论数量
	CreateDate    uint                `json:"create_date" db:"create_date"`       //创建日期
	CommentList   []Video365ygComment `json:"comment_list" db:"create_date"`      //评论列表
}

// 视频下面的评论列表
type Video365ygComment struct {
	AutoID    uint64 `json:"-" db:"auto_id"`             //无意义的自增ID
	CommentID uint64 `json:"comment_id" db:"comment_id"` //源评论ID
	GroupID   uint64 `json:"-" db:"group_id"`            //http://www.365yg.com/group/{group_id}/组成视频详情页
	Content   string `json:"content" db:"content"`       //评论内容
	Ctime     uint   `json:"ctime" db:"ctime"`           //发表评论的时间
}

func main() {
	sfile = "./videoid"
	myconf = negoutils.MySQLConf{
		Host:       "127.0.0.1",
		User:       "phpmyadmin",
		Passwd:     "123456",
		DbName:     "db_scrapy",
		Charset:    "utf8",
		Timeout:    5,
		Port:       3306,
		AutoCommit: true,
	}
	db = negoutils.NewDBase(myconf)
	_, err := db.Conn()
	if err != nil {
		fmt.Println(err)
		return
	}
	defer db.Close()

	//提取article
	articleFile := "./article"
	fp, err := negoutils.OpenNewFile(articleFile, ".bak", true)
	writer := bufio.NewWriter(fp)
	articleSQL := "SELECT * FROM `article` WHERE `auto_id` > ? LIMIT 100"
	articleAutoId := uint64(0)
	for {
		rows, err := db.FetchRows(articleSQL, articleAutoId)
		if err != nil {
			fmt.Println(err)
			return
		}
		if len(rows) == 0 {
			break
		}
		for row := range rows {
			b, e := json.Marshal(row)
			if e != nil {
				continue
			}
			var articleInfo Article
			e = json.Unmarshal(b, &articleInfo)
			if e != nil {
				continue
			}
			articleAutoId = articleInfo.AutoID
			b, e = json.Marshal(articleInfo)
			if e != nil {
				continue
			}
			writer.WriteString(negoutils.ByteToStr(b))
			writer.Flush()
		}
	}
	fp.Close()
	return
}
