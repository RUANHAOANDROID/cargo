package internal

import (
	"cargo/clib"
	"cargo/config"
	"cargo/pkg"
	"runtime"
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
			var memStats runtime.MemStats
			runtime.ReadMemStats(&memStats)
			M := memStats.Alloc / 1024 / 1024
			KB := memStats.Alloc / 1024
			pkg.Log.Printf("routines: %d, Memory: %d MB , %d KB\n", runtime.NumGoroutine(), M, KB)
		}
	}
}
