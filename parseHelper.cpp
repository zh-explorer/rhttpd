//
// Created by explorer on 2020/12/24.
//

#include "parseHelper.h"
#include <sys/time.h>
#include <sys/stat.h>
#include "globalContext.h"
#include <cstring>
#include <cstdio>
#include <cstdarg>


KeyMatch<char, enum Method, const char *> parseHelper::method_matcher = {
        {"GET",  3, get},
        {"POST", 4, post},
        {"HEAD", 4, head},
};

KeyMatch<char, char const *, char *> parseHelper::content_matcher = {
        {"tif",     3, "image/tiff"},
        {"001",     3, "application/x-001"},
        {"301",     3, "application/x-301"},
        {"323",     3, "text/h323"},
        {"906",     3, "application/x-906"},
        {"907",     3, "drawing/907"},
        {"a11",     3, "application/x-a11"},
        {"acp",     3, "audio/x-mei-aac"},
        {"ai",      2, "application/postscript"},
        {"aif",     3, "audio/aiff"},
        {"aifc",    4, "audio/aiff"},
        {"aiff",    4, "audio/aiff"},
        {"anv",     3, "application/x-anv"},
        {"asa",     3, "text/asa"},
        {"asf",     3, "video/x-ms-asf"},
        {"asp",     3, "text/asp"},
        {"asx",     3, "video/x-ms-asf"},
        {"au",      2, "audio/basic"},
        {"avi",     3, "video/avi"},
        {"awf",     3, "application/vnd.adobe.workflow"},
        {"biz",     3, "text/xml"},
        {"bmp",     3, "application/x-bmp"},
        {"bot",     3, "application/x-bot"},
        {"c4t",     3, "application/x-c4t"},
        {"c90",     3, "application/x-c90"},
        {"cal",     3, "application/x-cals"},
        {"cat",     3, "application/vnd.ms-pki.seccat"},
        {"cdf",     3, "application/x-netcdf"},
        {"cdr",     3, "application/x-cdr"},
        {"cel",     3, "application/x-cel"},
        {"cer",     3, "application/x-x509-ca-cert"},
        {"cg4",     3, "application/x-g4"},
        {"cgm",     3, "application/x-cgm"},
        {"cit",     3, "application/x-cit"},
        {"class",   5, "java/*"},
        {"cml",     3, "text/xml"},
        {"cmp",     3, "application/x-cmp"},
        {"cmx",     3, "application/x-cmx"},
        {"cot",     3, "application/x-cot"},
        {"crl",     3, "application/pkix-crl"},
        {"crt",     3, "application/x-x509-ca-cert"},
        {"csi",     3, "application/x-csi"},
        {"css",     3, "text/css"},
        {"cut",     3, "application/x-cut"},
        {"dbf",     3, "application/x-dbf"},
        {"dbm",     3, "application/x-dbm"},
        {"dbx",     3, "application/x-dbx"},
        {"dcd",     3, "text/xml"},
        {"dcx",     3, "application/x-dcx"},
        {"der",     3, "application/x-x509-ca-cert"},
        {"dgn",     3, "application/x-dgn"},
        {"dib",     3, "application/x-dib"},
        {"dll",     3, "application/x-msdownload"},
        {"doc",     3, "application/msword"},
        {"dot",     3, "application/msword"},
        {"drw",     3, "application/x-drw"},
        {"dtd",     3, "text/xml"},
        {"dwf",     3, "Model/vnd.dwf"},
        {"dwf",     3, "application/x-dwf"},
        {"dwg",     3, "application/x-dwg"},
        {"dxb",     3, "application/x-dxb"},
        {"dxf",     3, "application/x-dxf"},
        {"edn",     3, "application/vnd.adobe.edn"},
        {"emf",     3, "application/x-emf"},
        {"eml",     3, "message/rfc822"},
        {"ent",     3, "text/xml"},
        {"epi",     3, "application/x-epi"},
        {"eps",     3, "application/x-ps"},
        {"eps",     3, "application/postscript"},
        {"etd",     3, "application/x-ebx"},
        {"exe",     3, "application/x-msdownload"},
        {"fax",     3, "image/fax"},
        {"fdf",     3, "application/vnd.fdf"},
        {"fif",     3, "application/fractals"},
        {"fo",      2, "text/xml"},
        {"frm",     3, "application/x-frm"},
        {"g4",      2, "application/x-g4"},
        {"gbr",     3, "application/x-gbr"},
        {"",        0, "application/x-"},
        {"gif",     3, "image/gif"},
        {"gl2",     3, "application/x-gl2"},
        {"gp4",     3, "application/x-gp4"},
        {"hgl",     3, "application/x-hgl"},
        {"hmr",     3, "application/x-hmr"},
        {"hpg",     3, "application/x-hpgl"},
        {"hpl",     3, "application/x-hpl"},
        {"hqx",     3, "application/mac-binhex40"},
        {"hrf",     3, "application/x-hrf"},
        {"hta",     3, "application/hta"},
        {"htc",     3, "text/x-component"},
        {"htm",     3, "text/html"},
        {"html",    4, "text/html"},
        {"htt",     3, "text/webviewhtml"},
        {"htx",     3, "text/html"},
        {"icb",     3, "application/x-icb"},
        {"ico",     3, "image/x-icon"},
        {"ico",     3, "application/x-ico"},
        {"iff",     3, "application/x-iff"},
        {"ig4",     3, "application/x-g4"},
        {"igs",     3, "application/x-igs"},
        {"iii",     3, "application/x-iphone"},
        {"img",     3, "application/x-img"},
        {"ins",     3, "application/x-internet-signup"},
        {"isp",     3, "application/x-internet-signup"},
        {"IVF",     3, "video/x-ivf"},
        {"java",    4, "java/*"},
        {"jfif",    4, "image/jpeg"},
        {"jpe",     3, "image/jpeg"},
        {"jpe",     3, "application/x-jpe"},
        {"jpeg",    4, "image/jpeg"},
        {"jpg",     3, "image/jpeg"},
        {"jpg",     3, "application/x-jpg"},
        {"js",      2, "application/x-javascript"},
        {"jsp",     3, "text/html"},
        {"la1",     3, "audio/x-liquid-file"},
        {"lar",     3, "application/x-laplayer-reg"},
        {"latex",   5, "application/x-latex"},
        {"lavs",    4, "audio/x-liquid-secure"},
        {"lbm",     3, "application/x-lbm"},
        {"lmsff",   5, "audio/x-la-lms"},
        {"ls",      2, "application/x-javascript"},
        {"ltr",     3, "application/x-ltr"},
        {"m1v",     3, "video/x-mpeg"},
        {"m2v",     3, "video/x-mpeg"},
        {"m3u",     3, "audio/mpegurl"},
        {"m4e",     3, "video/mpeg4"},
        {"mac",     3, "application/x-mac"},
        {"man",     3, "application/x-troff-man"},
        {"math",    4, "text/xml"},
        {"mdb",     3, "application/msaccess"},
        {"mdb",     3, "application/x-mdb"},
        {"mfp",     3, "application/x-shockwave-flash"},
        {"mht",     3, "message/rfc822"},
        {"mhtml",   5, "message/rfc822"},
        {"mi",      2, "application/x-mi"},
        {"mid",     3, "audio/mid"},
        {"midi",    4, "audio/mid"},
        {"mil",     3, "application/x-mil"},
        {"mml",     3, "text/xml"},
        {"mnd",     3, "audio/x-musicnet-download"},
        {"mns",     3, "audio/x-musicnet-stream"},
        {"mocha",   5, "application/x-javascript"},
        {"movie",   5, "video/x-sgi-movie"},
        {"mp1",     3, "audio/mp1"},
        {"mp2",     3, "audio/mp2"},
        {"mp2v",    4, "video/mpeg"},
        {"mp3",     3, "audio/mp3"},
        {"mp4",     3, "video/mpeg4"},
        {"mpa",     3, "video/x-mpg"},
        {"mpd",     3, "application/vnd.ms-project"},
        {"mpe",     3, "video/x-mpeg"},
        {"mpeg",    4, "video/mpg"},
        {"mpg",     3, "video/mpg"},
        {"mpga",    4, "audio/rn-mpeg"},
        {"mpp",     3, "application/vnd.ms-project"},
        {"mps",     3, "video/x-mpeg"},
        {"mpt",     3, "application/vnd.ms-project"},
        {"mpv",     3, "video/mpg"},
        {"mpv2",    4, "video/mpeg"},
        {"mpw",     3, "application/vnd.ms-project"},
        {"mpx",     3, "application/vnd.ms-project"},
        {"mtx",     3, "text/xml"},
        {"mxp",     3, "application/x-mmxp"},
        {"net",     3, "image/pnetvue"},
        {"nrf",     3, "application/x-nrf"},
        {"nws",     3, "message/rfc822"},
        {"odc",     3, "text/x-ms-odc"},
        {"out",     3, "application/x-out"},
        {"p10",     3, "application/pkcs10"},
        {"p12",     3, "application/x-pkcs12"},
        {"p7b",     3, "application/x-pkcs7-certificates"},
        {"p7c",     3, "application/pkcs7-mime"},
        {"p7m",     3, "application/pkcs7-mime"},
        {"p7r",     3, "application/x-pkcs7-certreqresp"},
        {"p7s",     3, "application/pkcs7-signature"},
        {"pc5",     3, "application/x-pc5"},
        {"pci",     3, "application/x-pci"},
        {"pcl",     3, "application/x-pcl"},
        {"pcx",     3, "application/x-pcx"},
        {"pdf",     3, "application/pdf"},
        {"pdf",     3, "application/pdf"},
        {"pdx",     3, "application/vnd.adobe.pdx"},
        {"pfx",     3, "application/x-pkcs12"},
        {"pgl",     3, "application/x-pgl"},
        {"pic",     3, "application/x-pic"},
        {"pko",     3, "application/vnd.ms-pki.pko"},
        {"pl",      2, "application/x-perl"},
        {"plg",     3, "text/html"},
        {"pls",     3, "audio/scpls"},
        {"plt",     3, "application/x-plt"},
        {"png",     3, "image/png"},
        {"png",     3, "application/x-png"},
        {"pot",     3, "application/vnd.ms-powerpoint"},
        {"ppa",     3, "application/vnd.ms-powerpoint"},
        {"ppm",     3, "application/x-ppm"},
        {"pps",     3, "application/vnd.ms-powerpoint"},
        {"ppt",     3, "application/vnd.ms-powerpoint"},
        {"ppt",     3, "application/x-ppt"},
        {"pr",      2, "application/x-pr"},
        {"prf",     3, "application/pics-rules"},
        {"prn",     3, "application/x-prn"},
        {"prt",     3, "application/x-prt"},
        {"ps",      2, "application/x-ps"},
        {"ps",      2, "application/postscript"},
        {"ptn",     3, "application/x-ptn"},
        {"pwz",     3, "application/vnd.ms-powerpoint"},
        {"r3t",     3, "text/vnd.rn-realtext3d"},
        {"ra",      2, "audio/vnd.rn-realaudio"},
        {"ram",     3, "audio/x-pn-realaudio"},
        {"ras",     3, "application/x-ras"},
        {"rat",     3, "application/rat-file"},
        {"rdf",     3, "text/xml"},
        {"rec",     3, "application/vnd.rn-recording"},
        {"red",     3, "application/x-red"},
        {"rgb",     3, "application/x-rgb"},
        {"rjs",     3, "application/vnd.rn-realsystem-rjs"},
        {"rjt",     3, "application/vnd.rn-realsystem-rjt"},
        {"rlc",     3, "application/x-rlc"},
        {"rle",     3, "application/x-rle"},
        {"rm",      2, "application/vnd.rn-realmedia"},
        {"rmf",     3, "application/vnd.adobe.rmf"},
        {"rmi",     3, "audio/mid"},
        {"rmj",     3, "application/vnd.rn-realsystem-rmj"},
        {"rmm",     3, "audio/x-pn-realaudio"},
        {"rmp",     3, "application/vnd.rn-rn_music_package"},
        {"rms",     3, "application/vnd.rn-realmedia-secure"},
        {"rmvb",    4, "application/vnd.rn-realmedia-vbr"},
        {"rmx",     3, "application/vnd.rn-realsystem-rmx"},
        {"rnx",     3, "application/vnd.rn-realplayer"},
        {"rp",      2, "image/vnd.rn-realpix"},
        {"rpm",     3, "audio/x-pn-realaudio-plugin"},
        {"rsml",    4, "application/vnd.rn-rsml"},
        {"rt",      2, "text/vnd.rn-realtext"},
        {"rtf",     3, "application/msword"},
        {"rtf",     3, "application/x-rtf"},
        {"rv",      2, "video/vnd.rn-realvideo"},
        {"sam",     3, "application/x-sam"},
        {"sat",     3, "application/x-sat"},
        {"sdp",     3, "application/sdp"},
        {"sdw",     3, "application/x-sdw"},
        {"sit",     3, "application/x-stuffit"},
        {"slb",     3, "application/x-slb"},
        {"sld",     3, "application/x-sld"},
        {"slk",     3, "drawing/x-slk"},
        {"smi",     3, "application/smil"},
        {"smil",    4, "application/smil"},
        {"smk",     3, "application/x-smk"},
        {"snd",     3, "audio/basic"},
        {"sol",     3, "text/plain"},
        {"sor",     3, "text/plain"},
        {"spc",     3, "application/x-pkcs7-certificates"},
        {"spl",     3, "application/futuresplash"},
        {"spp",     3, "text/xml"},
        {"ssm",     3, "application/streamingmedia"},
        {"sst",     3, "application/vnd.ms-pki.certstore"},
        {"stl",     3, "application/vnd.ms-pki.stl"},
        {"stm",     3, "text/html"},
        {"sty",     3, "application/x-sty"},
        {"svg",     3, "text/xml"},
        {"swf",     3, "application/x-shockwave-flash"},
        {"tdf",     3, "application/x-tdf"},
        {"tg4",     3, "application/x-tg4"},
        {"tga",     3, "application/x-tga"},
        {"tif",     3, "image/tiff"},
        {"tif",     3, "application/x-tif"},
        {"tiff",    4, "image/tiff"},
        {"tld",     3, "text/xml"},
        {"top",     3, "drawing/x-top"},
        {"torrent", 7, "application/x-bittorrent"},
        {"tsd",     3, "text/xml"},
        {"txt",     3, "text/plain"},
        {"uin",     3, "application/x-icq"},
        {"uls",     3, "text/iuls"},
        {"vcf",     3, "text/x-vcard"},
        {"vda",     3, "application/x-vda"},
        {"vdx",     3, "application/vnd.visio"},
        {"vml",     3, "text/xml"},
        {"vpg",     3, "application/x-vpeg005"},
        {"vsd",     3, "application/vnd.visio"},
        {"vsd",     3, "application/x-vsd"},
        {"vss",     3, "application/vnd.visio"},
        {"vst",     3, "application/vnd.visio"},
        {"vst",     3, "application/x-vst"},
        {"vsw",     3, "application/vnd.visio"},
        {"vsx",     3, "application/vnd.visio"},
        {"vtx",     3, "application/vnd.visio"},
        {"vxml",    4, "text/xml"},
        {"wav",     3, "audio/wav"},
        {"wax",     3, "audio/x-ms-wax"},
        {"wb1",     3, "application/x-wb1"},
        {"wb2",     3, "application/x-wb2"},
        {"wb3",     3, "application/x-wb3"},
        {"wbmp",    4, "image/vnd.wap.wbmp"},
        {"wiz",     3, "application/msword"},
        {"wk3",     3, "application/x-wk3"},
        {"wk4",     3, "application/x-wk4"},
        {"wkq",     3, "application/x-wkq"},
        {"wks",     3, "application/x-wks"},
        {"wm",      2, "video/x-ms-wm"},
        {"wma",     3, "audio/x-ms-wma"},
        {"wmd",     3, "application/x-ms-wmd"},
        {"wmf",     3, "application/x-wmf"},
        {"wml",     3, "text/vnd.wap.wml"},
        {"wmv",     3, "video/x-ms-wmv"},
        {"wmx",     3, "video/x-ms-wmx"},
        {"wmz",     3, "application/x-ms-wmz"},
        {"wp6",     3, "application/x-wp6"},
        {"wpd",     3, "application/x-wpd"},
        {"wpg",     3, "application/x-wpg"},
        {"wpl",     3, "application/vnd.ms-wpl"},
        {"wq1",     3, "application/x-wq1"},
        {"wr1",     3, "application/x-wr1"},
        {"wri",     3, "application/x-wri"},
        {"wrk",     3, "application/x-wrk"},
        {"ws",      2, "application/x-ws"},
        {"ws2",     3, "application/x-ws"},
        {"wsc",     3, "text/scriptlet"},
        {"wsdl",    4, "text/xml"},
        {"wvx",     3, "video/x-ms-wvx"},
        {"xdp",     3, "application/vnd.adobe.xdp"},
        {"xdr",     3, "text/xml"},
        {"xfd",     3, "application/vnd.adobe.xfd"},
        {"xfdf",    4, "application/vnd.adobe.xfdf"},
        {"xhtml",   5, "text/html"},
        {"xls",     3, "application/vnd.ms-excel"},
        {"xls",     3, "application/x-xls"},
        {"xlw",     3, "application/x-xlw"},
        {"xml",     3, "text/xml"},
        {"xpl",     3, "audio/scpls"},
        {"xq",      2, "text/xml"},
        {"xql",     3, "text/xml"},
        {"xquery",  6, "text/xml"},
        {"xsd",     3, "text/xml"},
        {"xsl",     3, "text/xml"},
        {"xslt",    4, "text/xml"},
        {"xwd",     3, "application/x-xwd"},
        {"x_b",     3, "application/x-x_b"},
        {"sis",     3, "application/vnd.symbian.install"},
        {"sisx",    4, "application/vnd.symbian.install"},
        {"x_t",     3, "application/x-x_t"},
        {"ipa",     3, "application/vnd.iphone"},
        {"apk",     3, "application/vnd.android.package-archive"},
        {"xap",     3, "application/x-silverlight-app"}
};

std::map<int, const char *> parseHelper::status_phrase = {
        {100, "Continue"},
        {101, "Switching Protocols"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {307, "Temporary Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {426, "Upgrade Required"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"}
};


enum Method parseHelper::parse_method(const char *start, unsigned data_size) {
    try {
        return parseHelper::method_matcher.search(start, data_size);
    } catch (NotFindException &e) {
        return other;
    }
}

int parseHelper::find_space(const unsigned char *start, unsigned int data_size) {
    return find_ch(start, data_size, ' ');
}

int parseHelper::find_ch(const unsigned char *start, unsigned int data_size, unsigned char ch) {
    for (int i = 0; i < data_size; i++) {
        if (start[i] == ch) {
            return i;
        }
    }
    return -1;
}

const char *parseHelper::get_result_phrase(int status_code) {
    return parseHelper::status_phrase[status_code];
}

char const *parseHelper::parse_content_type(unsigned char const *url, unsigned int size) {
    unsigned int p = size - 1;
    while (p != 0 && url[p] != '/') {
        try {
            if (url[p] == '.') {
                return parseHelper::content_matcher.search((char *) (url + p + 1), size - 1 - p);
            }
            p--;
        } catch (NotFindException &e) {
            return "text/html";
        }
    }
    return "text/html";
}

// copy and parse form nginx source
void gmtime(time_t t, struct tm *tp) {
    int yday;
    unsigned int sec, min, hour, mday, mon, year, wday, days, leap;

    /* the calculation is valid for positive time_t only */

    if (t < 0) {
        t = 0;
    }

    days = t / 86400;
    sec = t % 86400;

    /*
     * no more than 4 year digits supported,
     * truncate to December 31, 9999, 23:59:59
     */

    if (days > 2932896) {
        days = 2932896;
        sec = 86399;
    }

    /* January 1, 1970 was Thursday */

    wday = (4 + days) % 7;

    hour = sec / 3600;
    sec %= 3600;
    min = sec / 60;
    sec %= 60;

    /*
     * the algorithm based on Gauss' formula,
     * see src/core/ngx_parse_time.c
     */

    /* days since March 1, 1 BC */
    days = days - (31 + 28) + 719527;

    /*
     * The "days" should be adjusted to 1 only, however, some March 1st's go
     * to previous year, so we adjust them to 2.  This causes also shift of the
     * last February days to next year, but we catch the case when "yday"
     * becomes negative.
     */

    year = (days + 2) * 400 / (365 * 400 + 100 - 4 + 1);

    yday = days - (365 * year + year / 4 - year / 100 + year / 400);

    if (yday < 0) {
        leap = (year % 4 == 0) && (year % 100 || (year % 400 == 0));
        yday = 365 + leap + yday;
        year--;
    }

    /*
     * The empirical formula that maps "yday" to month.
     * There are at least 10 variants, some of them are:
     *     mon = (yday + 31) * 15 / 459
     *     mon = (yday + 31) * 17 / 520
     *     mon = (yday + 31) * 20 / 612
     */

    mon = (yday + 31) * 10 / 306;

    /* the Gauss' formula that evaluates days before the month */

    mday = yday - (367 * mon / 12 - 30) + 1;

    if (yday >= 306) {

        year++;
        mon -= 10;

        /*
         * there is no "yday" in Win32 SYSTEMTIME
         *
         * yday -= 306;
         */

    } else {

        mon += 2;

        /*
         * there is no "yday" in Win32 SYSTEMTIME
         *
         * yday += 31 + 28 + leap;
         */
    }

    tp->tm_sec = sec;
    tp->tm_min = min;
    tp->tm_hour = hour;
    tp->tm_mday = mday;
    tp->tm_mon = mon;
    tp->tm_year = year;
    tp->tm_wday = wday;
}

static char *week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

char *parseHelper::get_date() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    time_t sec = tv.tv_sec;

    return parseHelper::http_time(sec);
}

char *parseHelper::http_time(time_t sec) {
    struct tm t;
    gmtime(sec, &t);

    char *buffer = static_cast<char *>(malloc(0x100));

    snprintf(buffer, 0x100, "%s, %02d %s %4d %02d:%02d:%02d GMT",
             week[t.tm_wday],
             t.tm_mday,
             months[t.tm_mon - 1],
             t.tm_year,
             t.tm_hour,
             t.tm_min,
             t.tm_sec);
    return buffer;
}

char *parseHelper::file_mtime(char *filename) {
    struct stat s;
    if (stat(filename, &s)) {
        return nullptr;
    }
    return parseHelper::http_time(s.st_mtime);
}

char *parseHelper::file_mtime(int fd) {
    struct stat s;
    if (fstat(fd, &s)) {
        return nullptr;
    }
    return parseHelper::http_time(s.st_mtime);
}


unsigned int parseHelper::get_file_length(FILE *fp) {
    int fd = fileno(fp);
    struct stat statbuf;
    fstat(fd, &statbuf);
    return statbuf.st_size;
}

std::tuple<unsigned char *, unsigned int> parseHelper::render(char *filename, unsigned int size, ...) {
    char *file_path = static_cast<char *>(malloc(globalContext::document_root_len + strlen(filename) + 1));
    strcpy(file_path, globalContext::document_root);
    strcat(file_path, filename);
    FILE *fp = fopen(file_path, "r");
    if (!fp) {
        return {nullptr, 0};
    }
    unsigned int file_len = parseHelper::get_file_length(fp);
    auto *out_buffer = static_cast<unsigned char *>(malloc(file_len + size));
    auto *buffer = static_cast<unsigned char *>(malloc(file_len));
    fread(buffer, 1, file_len, fp);

    va_list args;
    va_start(args, size);
    int re = vsnprintf(reinterpret_cast<char *>(out_buffer), file_len + size, reinterpret_cast<const char *>(buffer), args);
    free(buffer);

    return {out_buffer, re};
}



bool parseHelper::url_check(unsigned char *buffer, unsigned int size) {
    if (buffer == nullptr || size == 0) {
        return false;
    }
    for (unsigned int i = 0; i < size - 1; i++) {
        if (buffer[i] == '.' && buffer[i + 1] == '.') {
            return false;
        }
    }
    return true;
}

std::tuple<unsigned char *, unsigned int> parseHelper::url_decode(unsigned char *buffer, unsigned int size) {
    auto *url = static_cast<unsigned char *>(malloc(size));
    unsigned int i = 0;
    unsigned int j = 0;
    while (i < size) {
        if (buffer[i] != '%') {
//            if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
//                url[j] = buffer[i] + 32;
//            } else {
//                url[j] = buffer[i];
//            }
            if (buffer[i] == '+') {
                url[j] = ' ';
            } else {
                url[j] = buffer[i];
            }
            j++;
            i++;
        } else {
            if (i + 2 >= size) {
                // error, no enough char
                free(url);
                return {nullptr, 0};
            }

            // TODO: wtf
            unsigned char c;
            if (buffer[i + 1] >= '0' && buffer[i + 1] <= '9') {
                c = (buffer[i + 1] - '0') * 16;
            } else if (buffer[i + 1] >= 'a' && buffer[i + 1] <= 'z') {
                c = (buffer[i + 1] - 'a' + 10) * 16;
            } else if (buffer[i + 1] >= 'A' && buffer[i + 1] <= 'Z') {
                c = (buffer[i + 1] - 'A' + 10) * 16;
            } else {
                free(url);
                return {nullptr, 0};
            }

            if (buffer[i + 2] >= '0' && buffer[i + 2] <= '9') {
                c += (buffer[i + 2] - '0');
            } else if (buffer[i + 2] >= 'a' && buffer[i + 2] <= 'z') {
                c += (buffer[i + 2] - 'a' + 10);
            } else if (buffer[i + 2] >= 'A' && buffer[i + 2] <= 'Z') {
                c += (buffer[i + 2] - 'A' + 10);
            } else {
                free(url);
                return {nullptr, 0};
            }

            url[j] = c;
            j++;
            i += 3;
        }
    }
    return {url, j};
}