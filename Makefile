start:
	npm start

reason-watch:
	npm run reason-watch

reason-build:
	npm run reason-build

reason-clean:
	npm run reason-clean

test:
	npm run jest

test-watch:
	npm run jest -- --watchAll

test-coverage:
	npm run jest -- --coverage

webpack-build-dev:
	npm run webpack

webpack-build-prod:
	NODE_ENV=production npm run webpack

predeploy:
	make reason-build && make webpack-build-prod

update-babel:
	npm uninstal @babel/core && \
	npm uninstall @babel/preset-env && \
	npm i -D @babel/core && \
	npm i -D @babel/preset-env

update-webpack:
	npm uninstall webpack && \
	npm uninstall webpack-cli && \
	npm uninstall webpack-dev-server && \
	npm i -D webpack && \
	npm i -D webpack-cli && \
	npm i -D webpack-dev-server

update-webpack-plugins:
	npm uninstall clean-webpack-plugin && \
	npm uninstall html-webpack-plugin && \
	npm uninstall css-loader && \
	npm uninstall style-loader && \
	npm i -D clean-webpack-plugin && \
	npm i -D html-webpack-plugin && \
	npm i -D css-loader && \
	npm i -D style-loader
