package msg

type Message struct {
	Type    int
	Content interface{}
}

const (
	QRCODE  = 0
	ID_CARD = 1
	IC_CARD = 2
)
