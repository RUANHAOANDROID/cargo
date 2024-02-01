package msg

type Message struct {
	Type    int
	Content string
}

const (
	QRCODE  = 2
	IC_CARD = 1
)
