# rsync --dry-run --delete -av --exclude-from 'exclude-list.txt' ./ ../MixtCompExport
rsync -av -L --delete --delete-excluded --exclude-from 'exclude-list.txt' ./RMixtComp/ ../MixtCompRForge/pkg/

mkdir -p ../MixtCompRForge/pkg/src/MixtComp/bin/debug
touch ../MixtCompRForge/pkg/src/MixtComp/bin/debug/empty
mkdir -p ../MixtCompRForge/pkg/src/MixtComp/bin/release
touch ../MixtCompRForge/pkg/src/MixtComp/bin/release/empty
mkdir -p ../MixtCompRForge/pkg/src/MixtComp/lib
touch ../MixtCompRForge/pkg/src/MixtComp/lib/empty
mkdir -p ../MixtCompRForge/pkg/src/stkpp/bin
touch ../MixtCompRForge/pkg/src/stkpp/bin/empty
mkdir -p ../MixtCompRForge/pkg/src/stkpp/lib
touch ../MixtCompRForge/pkg/src/stkpp/lib/empty
