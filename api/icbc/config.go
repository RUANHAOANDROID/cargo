package icbc

import "cargo/config"

var conf *config.Config

func SetConfig(config *config.Config) {
	conf = config
}
