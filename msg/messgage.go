package msg

type Message struct {
	Type    int
	Content string
}

const (
	IC_CARD = 1
	QRCODE  = 2
	ID_CARD = 3
)
