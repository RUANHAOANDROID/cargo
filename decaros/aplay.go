package decaros

import (
	"cargo/pkg"
	"os/exec"
	"strings"
)

var (
	SoundFiles = []string{
		"00.wav", "01.wav", "02.wav", "03.wav", "04.wav", "05.wav", "06.wav", "07.wav", "08.wav", "09.wav",
		"7499.wav", "Amplified_rfidtestok.wav", "bai.wav", "canjrk.wav", "cikabunengdairenchengche.wav", "di.wav",
		"ding.wav", "dj.mp3", "erweimashixiao.wav", "feibaimingdanka.wav", "feifaerweiam.wav", "feifaka.wav",
		"feihutongka.wav", "feishoulika.wav", "fen.wav", "heimingdanka.wav", "huanyingshiyong.wav", "jiao.wav",
		"jiaoyiquxiao.wav", "jinglk.wav", "ksycsj.wav", "laorenka.wav", "mianfeika.wav", "nin.hao.wav",
		"piaojiaka.wav", "qcs.wav", "qhdsnm.wav", "qian.wav", "qiandaoshibai.wav", "qingchongsao.wav",
		"qingchongshua.wav", "qingchongzhi.wav", "qingshangche.wav", "qingshuasijikabingjianchashijian.wav",
		"qingtoubi.wav", "qingzhifu.wav", "qsh.wav", "qssjk.wav", "qssnm.wav", "qtb.wav", "rfidtestok.wav",
		"riqigeshiyichang.wav", "shebeijihuochenggong.wav", "shi.wav", "sijishangban.wav", "sijixiaban.wav",
		"skcg.wav", "sksb.wav", "upsjcg.wav", "wuxiaoka.wav", "wuxiaoma.wav", "wxm.wav", "xiexie.wav",
		"yebz.wav", "ysdcscg_real_lchanel.wav", "yuebuzu.wav", "yycs.wav", "zfcg.wav", "zfsb.wav",
		"zsdcscg_real_rchanel.wav",
	}
)

func APlay(wav string) {
	// 判断字符串是否包含 .wav 后缀
	if !strings.HasSuffix(wav, ".wav") {
		// 如果不包含，添加上 .wav 后缀
		wav += ".wav"
	}
	cmd := exec.Command("aplay", "/opt/sound/"+wav)

	cmd.Stderr = nil

	if err := cmd.Start(); err != nil {
		pkg.Log.Println("cmd run error:", err)
		return
	}
}
