type cell =
  | O
  | X;

type row = array(cell);

type grid = array(row);

type block = array(row);

type blockPosition = {
  x: int,
  y: int,
};

type blockSize = {
  width: int,
  height: int,
};

type nextBlock = block;

type gameOver = bool;

type intervalId = ref(option(Js.Global.intervalId));

type countdownId = ref(option(Js.Global.intervalId));

type countdownCounter = int;

type started = bool;

type gridState = {
  block : block,
  blockPosition: blockPosition,
  grid: grid,
};

type stats = {
  score: int,
  lines: int,
  level: int
};

type screen = Title | Counter | Game | Gameover;

type timerIds = {
  tick: intervalId,
  countdown: intervalId,
  moveLeft: intervalId,
  moveRight: intervalId,
  rotate: intervalId
};

type gameState = {
  gridState: gridState,
  stats: stats,
  gameOver: gameOver
};

type tickOutput = {
  gridState: gridState,
  stats: stats,
  gameOver: gameOver,
  nextBlockToShow: block
};

type globalState = {
  gridState: gridState,
  nextBlock: block,
  countdownCounter: countdownCounter,
  stats: stats,
  timerIds: timerIds,
  counting: bool,
  started: started,
  gameOver: gameOver,
  screen: screen,
  handleKeyboard: ref(unit => unit),
  unHandleKeyboard: ref(unit => unit)
};

type direction = Left | Right;
