package xhash

import (
	"github.com/jurgen-kluft/xcode/denv"
)

// GetPackage returns the package object of 'xhash'
func GetPackage() *denv.Package {

	// 'xhash' library
	mainlib := denv.SetupDefaultCppLibProject("xhash", "github.com\\jurgen-kluft\\xhash")

	// 'xhash' test project
	maintest := denv.SetupDefaultCppTestProject("xhash_test", "github.com\\jurgen-kluft\\xhash")

	mainpkg := denv.NewPackage("xhash")
	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
