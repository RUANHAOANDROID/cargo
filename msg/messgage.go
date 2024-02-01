package msg

type Message struct {
	Type    int
	Content interface{}
}

const (
	QRCODE  = 2
	IC_CARD = 1
)
