package internal

import (
	"cargo/clib"
	"cargo/config"
	"time"
)

func StartTimer(conf *config.Config, display *clib.Display) {
	display.ShowTime()
	timer := time.NewTimer(time.Minute)
	defer timer.Stop()
	for {
		timer.Reset(time.Minute) // 这里复用了 timer
		select {
		case <-timer.C:
			display.ShowTime()
		}
	}
}
