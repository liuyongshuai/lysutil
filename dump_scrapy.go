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
	Md5     string `json:"-" db:"md5"`           //段子的ID
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
		Timeout:    30,
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
	dumpVideo365yg()
	return
}

func dumpVideo365yg() {
	//提取article
	videoFile := "./video365yg"
	fp, err := negoutils.OpenNewFile(videoFile, ".bak", true)
	fmt.Println(err)
	writer := bufio.NewWriter(fp)
	videoSQL := "SELECT * FROM `video_365yg` WHERE `auto_id` > ? ORDER BY `auto_id` ASC LIMIT 100"
	commentSQL := "SELECT * FROM `video_365yg_comment` WHERE `group_id` = ? AND `auto_id` > ? ORDER BY `auto_id` ASC LIMIT 1000"
	videoAutoId := uint64(0)
	commentAutoId := uint64(0)
	for {
		fmt.Println("videoAutoId=", videoAutoId)
		rows, err := db.FetchRows(videoSQL, videoAutoId)
		if err != nil {
			fmt.Println(err)
			return
		}
		if len(rows) == 0 {
			break
		}

		for _, row := range rows {
			var videoInfo Video365yg
			videoInfo.AutoID, _ = row["auto_id"].ToUint64()
			videoInfo.GroupID, _ = row["group_id"].ToUint64()
			videoInfo.Title = row["title"].ToString()
			videoInfo.VideoDuration, _ = row["video_duration"].ToUint()
			videoInfo.VideoID = row["video_id"].ToString()
			videoInfo.VideoDesc = row["video_desc"].ToString()
			videoInfo.CommentNum, _ = row["comment_num"].ToUint()
			videoInfo.CreateDate, _ = row["create_date"].ToUint()
			videoAutoId = videoInfo.AutoID

			//提取评论信息
			for {
				commentRows, commentErr := db.FetchRows(commentSQL, videoInfo.GroupID, commentAutoId)
				if commentErr != nil || len(commentRows) == 0 {
					break
				}
				for _, commentRow := range commentRows {
					var commentInfo Video365ygComment
					commentInfo.AutoID, _ = commentRow["auto_id"].ToUint64()
					commentInfo.CommentID, _ = commentRow["comment_id"].ToUint64()
					commentInfo.GroupID, _ = commentRow["group_id"].ToUint64()
					commentInfo.Content = commentRow["content"].ToString()
					commentInfo.Ctime, _ = commentRow["ctime"].ToUint()
					commentAutoId = commentInfo.AutoID
					videoInfo.CommentList = append(videoInfo.CommentList, commentInfo)
				}
			}
			b, e := json.Marshal(videoInfo)
			if e != nil {
				fmt.Println(e)
				continue
			}
			writer.WriteString(negoutils.ByteToStr(b) + "\n")
			writer.Flush()
		}
	}
	fp.Close()
}

func dumpJokes() {
	//提取article
	articleFile := "./joke_text"
	fp, err := negoutils.OpenNewFile(articleFile, ".bak", true)
	fmt.Println(err)
	writer := bufio.NewWriter(fp)
	articleSQL := "SELECT * FROM `joke_text` WHERE `auto_id` > ? LIMIT 100"
	jokeAutoId := uint64(0)
	for {
		fmt.Println("jokeAutoId=", jokeAutoId)
		rows, err := db.FetchRows(articleSQL, jokeAutoId)
		if err != nil {
			fmt.Println(err)
			return
		}
		if len(rows) == 0 {
			break
		}
		for _, row := range rows {
			var jokeInfo JokeText
			jokeInfo.AutoID, _ = row["auto_id"].ToUint64()
			jokeInfo.Md5 = row["md5"].ToString()
			jokeInfo.Content = row["content"].ToString()
			jokeAutoId = jokeInfo.AutoID
			b, e := json.Marshal(jokeInfo)
			if e != nil {
				fmt.Println(e)
				continue
			}
			writer.WriteString(negoutils.ByteToStr(b) + "\n")
			writer.Flush()
		}
	}
	fp.Close()
}

func dumpArticle() {
	//提取article
	articleFile := "./article"
	fp, err := negoutils.OpenNewFile(articleFile, ".bak", true)
	fmt.Println(err)
	writer := bufio.NewWriter(fp)
	articleSQL := "SELECT * FROM `article` WHERE `auto_id` > ? LIMIT 100"
	articleAutoId := uint64(0)
	for {
		fmt.Println("articleAutoId=", articleAutoId)
		rows, err := db.FetchRows(articleSQL, articleAutoId)
		if err != nil {
			fmt.Println(err)
			return
		}
		if len(rows) == 0 {
			break
		}
		for _, row := range rows {
			var articleInfo Article
			articleInfo.AutoID, _ = row["auto_id"].ToUint64()
			articleInfo.Tags = row["tags"].ToString()
			articleInfo.Content = row["content"].ToString()
			articleInfo.Title = row["title"].ToString()
			articleInfo.URL = row["url"].ToString()
			articleAutoId = articleInfo.AutoID
			b, e := json.Marshal(articleInfo)
			if e != nil {
				fmt.Println(e)
				continue
			}
			writer.WriteString(negoutils.ByteToStr(b) + "\n")
			writer.Flush()
		}
	}
	fp.Close()
}
