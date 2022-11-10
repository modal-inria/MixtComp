.PHONY: help build build-dev notebook notebook-dev dev run
.DEFAULT_GOAL := help

ALL_IMAGES:=pyMixtComp/python/pyMixtComp

# Docker image build info
PROJECT:=pymixtcomp
export PROJECT
BUILD_TAG?=latest

# Port to expose for Jupyter when running the container
PORT=8888
export PORT

help:
# http://marmelab.com/blog/2016/02/29/auto-documented-makefile.html
	@echo "MixtComp repo"
	@echo "====================="
	@echo
	@grep -E '^[a-zA-Z0-9_%/-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'


########################################################
## Local development
########################################################


#### Installing the repo ####
install: ## Install the repo locally
	pip install -r ./pyMixtComp/python/requirements.txt
	jupyter labextension install --minimize=False @jupyter-widgets/jupyterlab-manager  arcgis-map-ipywidget@v1.8.2

########################################################
## Docker commands for Python
########################################################


#### Building the image ####
# Erase intermediate containers
build: ERASEARGS=--rm --force-rm  ## Build the image
# Don't erase intermediate containers - speeds up the development cycle
build-dev: ERASEARGS=  ## Build the image (development version)
build build-dev:
	docker build $(ERASEARGS) -t $(PROJECT):${BUILD_TAG} --file ./Dockerfile .


#### Running Jupyter notebook ####
# Clean up the container when finished
notebook: ERASEARGS=--rm ## Run a Jupyter notebook
# Don't clean up the container when finished - useful for development
notebook-dev: ERASEARGS= ## Run a Jupyter notebook (development version)
notebook notebook-dev: DARGS?=-v ${CURDIR}/pyMixtComp/python -p $(PORT):8888
notebook notebook-dev:
# docker run $(ERASEARGS) $(MEMARGS) $(DARGS) $(PROJECT)
# docker run -v ${CURDIR}/pyMixtComp/python/data:/pyMixtComp/python/data $(ERASEARGS) $(MEMARGS) $(DARGS) $(PROJECT)
	docker run -it --mount src="${CURDIR}/pyMixtComp",target="/MixtComp/pyMixtComp",type=bind $(ERASEARGS) $(MEMARGS) $(DARGS) $(PROJECT)