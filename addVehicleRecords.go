// @author      Liu Yongshuai<liuyongshuai@hotmail.com>
// @file        addVehicleRecords.go
// @date        2025-05-13 16:24

package main

import (
	"context"
	"encoding/json"
	"fmt"
	"github.com/liuyongshuai/negoutils"
	"time"
)

type VehicleCheckInfo struct {
	Key    string `json:"key"`
	Status int    `json:"status"`
	Value  string `json:"value"`
}

type VehicleInfo struct {
	DateOfUse              string             `json:"dateOfUse"`
	StartTime              string             `json:"startTime"`
	EndTime                string             `json:"endTime"`
	Purpose                string             `json:"purpose"`
	UsageRecord            string             `json:"usageRecord"`
	ParkingPosition        string             `json:"parkingPosition"`
	PositionDetail         string             `json:"positionDetail"`
	ProblemOrNot           int                `json:"problemOrNot"`
	RecordProblemPic       string             `json:"recordProblemPic"`
	ChangePartOrNot        int                `json:"changePartOrNot"`
	OverwriteSoftwareOrNot int                `json:"overwriteSoftwareOrNot"`
	OdometerDisplay        int                `json:"odometerDisplay"`
	IdleDuration           int                `json:"idleDuration"`
	VehicleChecks          []VehicleCheckInfo `json:"vehicleChecks"`
	VehicleVrmtcNum        string             `json:"vehicleVrmtcNum"`
}

var (
	url              = "https://tvms.gwm.cn/api/vrmtc-vehicle/record/addVehicleRecords"
	startTimestamp   = int64(1748736000)
	endTimestamp     = int64(1767139200)
	CYS              = []string{"CYS-8879", "CYS-7834", "CYS-7839", "CYS-7844"}
	vehicleCheckInfo = []VehicleCheckInfo{
		{"10", 0, "XB1无异常无脱落（异常处理联系张天齐）"},
		{"20", 0, "关闭车窗车门"},
		{"30", 0, "车内5S良好无杂物"},
		{"40", 0, "CYS标识正常未脱落"},
	}
	purpose        = "HP370平台开发"
	positionDetail = "北京市海淀区奥北科技园地上停车场"
	usageRecord    = "泊车测试"
)

func main() {
	for _, cys := range CYS {
		addVehicleRecords(cys)
	}
}

func addVehicleRecords(cys string) error {
	vehicleInfos := make([]VehicleInfo, 0)
	for s := startTimestamp; s <= endTimestamp; s += 86400 {
		formattedTime := time.Unix(s, 0).Format("2006-01-02")
		fmt.Println(formattedTime, cys)
		vehicleInfos = append(vehicleInfos, VehicleInfo{
			DateOfUse:              formattedTime,
			StartTime:              "10:00",
			EndTime:                "18:00",
			Purpose:                purpose,
			UsageRecord:            usageRecord,
			ParkingPosition:        "40",
			PositionDetail:         positionDetail,
			ProblemOrNot:           1,
			RecordProblemPic:       "",
			ChangePartOrNot:        1,
			OverwriteSoftwareOrNot: 1,
			OdometerDisplay:        26874,
			IdleDuration:           60,
			VehicleChecks:          vehicleCheckInfo,
			VehicleVrmtcNum:        cys,
		})
	}
	j, e := json.Marshal(vehicleInfos)
	if e != nil {
		fmt.Println(e)
		return e
	}

	//开始提交请求
	client := negoutils.NewHttpClient(url, context.Background())
	client.SetReferer("https://tvms.gwm.cn//")
	client.SetTimeout(100000 * time.Second)
	client.SetUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/136.0.0.0 Safari/537.36")
	client.AddHeader("Accept", "application/json, text/plain, */*")
	client.AddHeader("Accept-Encoding", "gzip, deflate, br, zstd")
	client.AddHeader("Accept-Language", "zh-CN,zh;q=0.9")
	client.AddHeader("AccessToken", "b0b02f73-1079-4f9a-8240-7472fc0b7011")
	client.AddHeader("Authorization", "b0b02f73-1079-4f9a-8240-7472fc0b7011")
	client.AddHeader("Content-Type", "application/json;charset=UTF-8")
	client.AddHeader("Cookie", "Access-Token=b0b02f73-1079-4f9a-8240-7472fc0b7011")
	client.AddHeader("Origin", "https://tvms.gwm.cn")
	client.AddHeader("Platform_Code", "2de2d58c112e400d6ce53d40fb74ac4a")
	client.AddHeader("Priority", "u=1, i")
	client.AddHeader("sec-ch-ua", "\"Chromium\";v=\"136\", \"Google Chrome\";v=\"136\", \"Not.A/Brand\";v=\"99\"")
	client.AddHeader("sec-ch-ua-mobile", "?0")
	client.AddHeader("sec-ch-ua-platform", "macOS")
	client.AddHeader("sec-fetch-dest", "empty")
	client.AddHeader("sec-fetch-mode", "cors")
	client.AddHeader("sec-fetch-site", "same-origin")
	client.SetKeepAlive(false)
	client.SetRawRequestBody(j)
	client.Post()
	return nil
}
