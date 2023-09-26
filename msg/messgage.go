package msg

type Message struct {
	Type    int
	Content interface{}
}

const (
	QRCODE  = 0x00
	ID_CARD = 0x01
	IC_CARD = 0x02
)
