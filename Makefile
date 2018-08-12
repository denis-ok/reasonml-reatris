build:
	npm run clean
	npm run build

start:
	npm run start

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

webpack-dev-server:
	npm run webpack-dev-server -- --open --hot

webpack-build-dev:
	npm run webpack

webpack-build-prod:
	NODE_ENV=production npm run webpack
