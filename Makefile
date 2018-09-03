reason-watch:
	npm run start

reason-build:
	npm run clean
	npm run build

reason-clean:
	npm run clean

test:
	npm run jest

test-watch:
	npm run jest -- --watchAll

test-coverage:
	npm run jest -- --coverage

webpack-watch:
	npm run webpack -w

webpack-serve:
	npm run webpack-serve -- --config webpack.config.js --open

# webpack-dev-server:
# 	npm run webpack-dev-server -- --open --hot

webpack-build-dev:
	npm run webpack

webpack-build-prod:
	NODE_ENV=production npm run webpack

predeploy:
	make reason-build
	make webpack-build-prod

deploy:
	npm run gh-pages -- -d build
