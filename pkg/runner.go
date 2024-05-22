package pkg

import "runtime"

func RunInfo() {
	var memStats runtime.MemStats
	runtime.ReadMemStats(&memStats)
	M := memStats.Alloc / 1024 / 1024
	KB := memStats.Alloc / 1024
	Log.Printf("routines: %d, Memory: %d MB , %d KB\n", runtime.NumGoroutine(), M, KB)
}
