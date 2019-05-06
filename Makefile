start:
	npm start

reason-watch:
	npm run reason-watch

reason-build:
	npm run build

reason-clean:
	npm run clean

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
	npm run build && make webpack-build-prod

deploy:
	make predeploy && npm run gh-pages -- -d build
