open Types;

let getWidth = (grid: grid) => Array.length(grid[0]);
let getHeight = (grid: grid) => Array.length(grid);

let isValidPosition = (position: blockPosition, block: block, grid: grid) => {
  let {x, y} = position;

  let blockWidth = getWidth(block);
  let blockHeight = getHeight(block);

  let gridWidth = getWidth(grid);
  let gridHeight = getHeight(grid);

  let isValidX = x >= 0 && x <= gridWidth - blockWidth;
  let isValidY = y >= 0 && y <= gridHeight - blockHeight;

  isValidX && isValidY;
};

let canMapRow = (posX: int, blockRow: row, gridRow: row) =>
  blockRow->Belt.Array.reduceWithIndex(_, true, (acc, blockCell, i) =>
              switch (acc) {
              | false => false
              | true =>
                let gridCell = gridRow[posX + i];
                switch (blockCell, gridCell) {
                | (O, O) => true
                | (X, O) => true
                | (O, X) => true
                | (X, X) => false
                };
              }
            );

let canMapBlock = (position: blockPosition, block: block, grid: grid) => {
  let validPosition = isValidPosition(position, block, grid);

  if (validPosition) {
    block->Belt.Array.reduceWithIndex(_, true, (acc, blockRow, i) =>
             switch (acc) {
             | false => false
             | true =>
               let gridRow = grid[position.y + i];
               gridRow->canMapRow(position.x, blockRow, _);
             }
           );
  } else {
    false;
  };
};

let mapBlockRowToGridRow = (posX: int, blockRow: row, gridRow: row) => {
  let newGridRow = Belt.Array.copy(gridRow);

  blockRow->Belt.Array.forEachWithIndex((i, blockCell) =>
    switch (blockCell) {
    | O => ()
    | X => newGridRow[posX + i] = blockCell
    }
  );

  newGridRow;
};

let mapBlockToGrid = ({blockPosition, block, grid}: gridState) => {
  let newGrid = Belt.Array.copy(grid);

  block->Belt.Array.forEachWithIndex((i, blockRow) => {
    let gridRowIndex = blockPosition.y + i;
    newGrid[gridRowIndex] =
      mapBlockRowToGridRow(blockPosition.x, blockRow, grid[gridRowIndex]);
  });

  newGrid;
};

let getStrokeIndexes = (grid: grid) => {
  let checkRow = gridRow =>
    gridRow->Belt.Array.reduce(true, (acc, elem) =>
      switch (acc, elem) {
      | (true, X) => true
      | (_, _) => false
      }
    );

  grid
  ->Belt.Array.reduceWithIndex(
      [],
      (acc, gridRow, i) => {
        let hasStroke = checkRow(gridRow);
        hasStroke ? [i, ...acc] : acc;
      },
    )
  ->Belt.List.toArray;
};

let removeFilledRows = (grid: grid, indexes) => {
  let indexesCount = Array.length(indexes);

  if (indexesCount == 0) {
    grid;
  } else {
    let hasIndex = i => Js.Array.includes(i, indexes);
    let emptyRow = Belt.Array.make(Array.length(grid[0]), O);

    let filteredGrid =
      grid->Belt.Array.keepWithIndex((_el, i) => !hasIndex(i));

    filteredGrid->Belt.Array.concat(
                    Belt.Array.make(indexesCount, emptyRow),
                    _,
                  );
  };
};

let genInitBlockPosition = (~block: block, ~gridWidth: int): blockPosition => {
  let blockWidth = getWidth(block);
  let blockHeight = getHeight(block);

  let posX = Random.int(gridWidth - blockWidth);
  let posY = 3 - blockHeight;

  let position = {x: posX, y: posY};
  position;
};

let genInitGridState = (~gridWidth: int, ~gridHeight: int): gridState => {
  let emptyRow = Array.make(gridWidth, O);
  let emptyGrid = Array.make(gridHeight, emptyRow);

  let firstBlock = Blocks.getRandomBlock();
  let initBlockPosition = genInitBlockPosition(~block=firstBlock, ~gridWidth);

  {block: firstBlock, blockPosition: initBlockPosition, grid: emptyGrid};
};

let calcLevel = score => {
  let divided = score / 500;
  Js.Math.floor(float_of_int(divided)) + 1;
};

let calcNextStats = (~stats: stats, ~strokesCount) => {
  let {score, lines, level} = stats;

  let strokesBonus =
    switch (strokesCount) {
    | 0 => 0
    | 1 => 50
    | 2 => 150
    | 3 => 250
    | _ => 350
    };

  let newScore = score + strokesBonus + 1;

  {
    score: newScore,
    lines: lines + strokesCount,
    level: strokesCount > 0 ? calcLevel(newScore) : level,
  };
};

let tick = (gridState: gridState, stats: stats, ~nextBlock=?, ()) => {
  let {blockPosition, block, grid}: gridState = gridState;
  let {x, y} = blockPosition;
  let nextPosition = {x, y: y + 1};

  let canMap = canMapBlock(nextPosition, block, grid);

  let nextBlock =
    switch (nextBlock) {
    | Some(block) => block
    | None => Blocks.getRandomBlock()
    };

  if (canMap) {
    let nextStats = calcNextStats(~stats, ~strokesCount=0);
    let nextGridState: gridState = {
      ...gridState,
      blockPosition: nextPosition,
    };

    {
      gridState: nextGridState,
      stats: nextStats,
      gameOver: false,
      nextBlockToShow: nextBlock,
    };
  } else {
    let nextGrid = mapBlockToGrid({blockPosition, block, grid});
    let strokeIndexes = getStrokeIndexes(nextGrid);
    let nextGridWithRemovedRows = removeFilledRows(nextGrid, strokeIndexes);

    let nextStats =
      calcNextStats(~stats, ~strokesCount=Array.length(strokeIndexes));

    let nextGridState = {
      block: nextBlock,
      blockPosition:
        genInitBlockPosition(~block=nextBlock, ~gridWidth=getWidth(grid)),
      grid: nextGridWithRemovedRows,
    };

    let gameOver = {
      let blockHeight = getHeight(nextBlock);
      let {y} = nextPosition;
      y == 3 - blockHeight && !canMap;
    };

    {
      gridState: nextGridState,
      stats: nextStats,
      gameOver,
      nextBlockToShow: Blocks.getRandomBlock(),
    };
  };
};

let getNextPositionByDirection = (direction, currentPosition) => {
  let {x, y} = currentPosition;

  let nextPosition =
    switch (direction) {
    | Left => {x: x - 1, y}
    | Right => {x: x + 1, y}
    };

  nextPosition;
};

let getGridStateAfterMove = (direction: direction, gridState) => {
  let {blockPosition, block, grid} = gridState;

  let nextPosition = getNextPositionByDirection(direction, blockPosition);

  let canMap = canMapBlock(nextPosition, block, grid);

  canMap ? {...gridState, blockPosition: nextPosition} : gridState;
};

let rotate2dArr = (arr: array(array('a))) => {
  let initArr = arr[0];

  let getColumn = (i, grid) => grid->Belt.Array.map(row => row[i]);

  initArr->Belt.Array.mapWithIndex((i, _row) => getColumn(i, arr));
};

let rotateClockwise = grid => grid->Belt.Array.reverse->rotate2dArr;

let getGridStateAfterRotate = gridState => {
  let {blockPosition, block, grid} = gridState;
  let rotatedBlock = rotateClockwise(block);

  let canMap = canMapBlock(blockPosition, rotatedBlock, grid);

  canMap ? {...gridState, block: rotatedBlock} : gridState;
};

let calcDelay = level =>
  switch (level) {
  | 1 => 600
  | 2 => 550
  | 3 => 500
  | 4 => 450
  | 5 => 400
  | 6 => 350
  | 7 => 300
  | 8 => 250
  | 9 => 200
  | 10 => 150
  | 11 => 140
  | 12 => 130
  | 13 => 120
  | 14 => 110
  | 15 => 100
  | 16 => 90
  | 17 => 80
  | 18 => 70
  | 19 => 60
  | 20 => 50
  | _ => 40
  };
