package chash

import (
	cbase "github.com/jurgen-kluft/cbase/package"
	denv "github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// GetPackage returns the package object of 'chash'
func GetPackage() *denv.Package {
	// Dependencies
	cunittestpkg := cunittest.GetPackage()
	cbasepkg := cbase.GetPackage()

	// The main (chash) package
	mainpkg := denv.NewPackage("chash")
	mainpkg.AddPackage(cunittestpkg)
	mainpkg.AddPackage(cbasepkg)

	// 'chash' library
	mainlib := denv.SetupDefaultCppLibProjectWithLibs("chash", "github.com\\jurgen-kluft\\chash", getPlatformLibs())
	mainlib.Dependencies = append(mainlib.Dependencies, cbasepkg.GetMainLib())

	// 'chash' unittest project
	maintest := denv.SetupDefaultCppTestProject("chash_test", "github.com\\jurgen-kluft\\chash")
	maintest.Dependencies = append(maintest.Dependencies, cunittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}

func getPlatformLibs() []*denv.Lib {
	if denv.IsWindows() {
		winLibs := []*denv.Lib{
			{Configs: denv.ConfigTypeAll, Type: denv.SystemLibrary, Files: []string{"kernel32.lib", "user32.lib", "gdi32.lib", "comdlg32.lib", "advapi32.lib"}},
		}
		return winLibs
	}
	if denv.IsMacOS() {
		macLibs := []*denv.Lib{
			{Configs: denv.ConfigTypeAll, Type: denv.Framework, Files: []string{"Cocoa", "Metal", "OpenGL", "IOKit", "Carbon", "CoreVideo", "QuartzCore"}},
		}
		return macLibs
	}
	return []*denv.Lib{}
}
