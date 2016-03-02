package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xhash/package"
)

func main() {
	xcode.Generate(xhash.GetProject())
}
