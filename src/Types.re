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

module Timer = {
  type t =
    | Tick
    | Countdown
    | MoveLeft
    | MoveRight
    | Rotate;

  let every = [Tick, Countdown, MoveLeft, MoveRight, Rotate];

  module State = {
    type t = {
      tick: option(Js.Global.intervalId),
      countdown: option(Js.Global.intervalId),
      moveLeft: option(Js.Global.intervalId),
      moveRight: option(Js.Global.intervalId),
      rotate: option(Js.Global.intervalId),
    };

    let empty = {
      tick: None,
      countdown: None,
      moveLeft: None,
      moveRight: None,
      rotate: None,
    };
  };
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
    gameOver: bool,
    nextBlockToShow: block,
  };
};

module GlobalState = {
  type t = {
    screen: Screen.t,
    countdownCounter: int,
    stats: GameStats.t,
    gridState: GridState.t,
    nextBlock: block,
    timers: Timer.State.t,
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
