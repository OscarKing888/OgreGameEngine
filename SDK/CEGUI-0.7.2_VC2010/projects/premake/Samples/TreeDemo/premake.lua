--
-- CEGUI sample premake script
--
sample("TreeDemo")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(rootdir..pkgdir)
