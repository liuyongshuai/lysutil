namespace cpp parking.uploadfile


//上传文件相关
struct uploadFileMetaInfoRequest{
    1: required string md5;
	2: required string raw_filename;
	3: required string upload_path;     //上传后在服务端的绝对路径
	4: required i64 filesize;
}

struct uploadFileSegmentInfoRequest{
    1: required string md5;
    2: required i64 segment_id;
    3: required i64 segmeng_size;
    4: required binary segmeng_content;
}

struct uploadFileFinishRequest{
    1: required string md5;
}

struct deleteFileRequest{
    1: required string filename;    //可以是目录或者文件，为目录时会递归清理目录下的文件
}

struct uploadStatisticResponse{
    1: required list<i64> finish_segment_ids;   //已完成上传的片断ID列表
    2: required list<i64> unfinish_segment_ids; //未上传完的片断ID列表
}

struct fileInfo{
    1: required string filename;
    2: required string upload_user;
    3: required i64 upload_time;
    4: required i64 filesize;
    5: optional list<string> tags;  //文件的标签，如"302"、"规控"、"林奥地下"
}

struct dirInfo{
    1: required string dirname; //最底层的目录名称，如"/data/bag/集成测试/0312"，在展现时会按照/切开
    2: required list<fileInfo> filelist;
}

struct fileListResponse{
    1: required string root_dir;    //服务端的根目录
    2: required list<dirInfo> dirlist;
}

service uploadFileService {
    //上传初始化操作
    uploadStatisticResponse initUpload(1: uploadFileMetaInfoRequest req);
    //上传文件内容
    uploadStatisticResponse uploadSegment(1: uploadFileSegmentInfoRequest req);
    //获取上传统计信息
    uploadStatisticResponse getUploadStatistic(1: string md5);
    //完成上传
    void finishUpload(1: uploadFileFinishRequest req);
    //删除目录或者文件
    void deleteFile(1: deleteFileRequest req);
    //所有的文件列表
    fileListResponse getFileList();
}


