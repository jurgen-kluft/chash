package xhash

import (
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode/denv"
)

// GetProject returns the Project information of 'xhash'
func GetProject() *denv.Project {
	denv.SetupDefaultCppProject("xhash", "github.com\\jurgen-kluft")
	project.Dependencies = append(project.Dependencies, xbase.GetProject())

	return project
}
