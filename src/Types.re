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

type timerIds = {
  tick: intervalId,
  countdown: intervalId,
  moveLeft: intervalId,
  moveRight: intervalId,
  rotate: intervalId,
};

module GridState = {
  type t = {
    block,
    blockPosition,
    grid,
  };
};

module GameStats = {
  type t = {
    score: int,
    lines: int,
    level: int,
  };
};

module Screen = {
  type t =
    | Title
    | Counter
    | Game
    | Gameover;
};

module TickOutput = {
  type t = {
    gridState: GridState.t,
    stats: GameStats.t,
    gameOver,
    nextBlockToShow: block,
  };
};

module GlobalState = {
  type t = {
    gridState: GridState.t,
    nextBlock: block,
    stats: GameStats.t,
    gameOver,
  };
};

module Direction = {
  type t =
    | Left
    | Right;
};

module KeyboardButton = {
  type t =
    | ArrowUp
    | ArrowRight
    | ArrowDown
    | ArrowLeft
    | Unsupported;

  let fromString =
    fun
    | "ArrowUp" => ArrowUp
    | "ArrowRight" => ArrowRight
    | "ArrowDown" => ArrowDown
    | "ArrowLeft" => ArrowLeft
    | _ => Unsupported;
};
