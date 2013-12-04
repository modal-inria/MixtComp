# rsync --dry-run --delete -av --exclude-from 'exclude-list.txt' ./ ../MixtCompExport
rsync -av -L --delete --delete-excluded --exclude-from 'exclude-list.txt' ./RMixtComp/ ../RMixtCompExport

mkdir -p ../RMixtCompExport/src/MixtComp/bin/debug
touch ../RMixtCompExport/src/MixtComp/bin/debug/empty
mkdir -p ../RMixtCompExport/src/MixtComp/bin/release
touch ../RMixtCompExport/src/MixtComp/bin/release/empty
mkdir -p ../RMixtCompExport/src/MixtComp/lib
touch ../RMixtCompExport/src/MixtComp/lib/empty
mkdir -p ../RMixtCompExport/src/stkpp/bin
touch ../RMixtCompExport/src/stkpp/bin/empty
mkdir -p ../RMixtCompExport/src/stkpp/lib
touch ../RMixtCompExport/src/stkpp/lib/empty
