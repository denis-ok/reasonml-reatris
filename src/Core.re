open Types;
open Belt;

let getWidth = (grid: grid) => grid->Array.getUnsafe(0)->Array.size;

let getHeight = (grid: grid) => Array.size(grid);

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
  blockRow->Array.reduceWithIndex(_, true, (acc, blockCell, i) =>
              switch (acc) {
              | false => false
              | true =>
                let gridCell = Array.getUnsafe(gridRow, posX + i);
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
    block->Array.reduceWithIndex(_, true, (acc, blockRow, i) =>
             switch (acc) {
             | false => false
             | true =>
               let gridRow = Array.getUnsafe(grid, position.y + i);
               gridRow->canMapRow(position.x, blockRow, _);
             }
           );
  } else {
    false;
  };
};

let mapBlockRowToGridRow = (~posX: int, ~blockRow: row, ~gridRow: row) => {
  let newGridRow = Array.copy(gridRow);

  blockRow->Array.forEachWithIndex((i, blockCell) =>
    switch (blockCell) {
    | O => ()
    | X => Array.setUnsafe(newGridRow, posX + i, blockCell)
    }
  );

  newGridRow;
};

let mapBlockToGrid = ({blockPosition, block, grid}: GridState.t) => {
  let newGrid = Array.copy(grid);

  block->Array.forEachWithIndex((i, blockRow) => {
    let gridRowIndex = blockPosition.y + i;

    let currentGridRow = Array.getUnsafe(grid, gridRowIndex);

    let newGridRow =
      mapBlockRowToGridRow(
        ~posX=blockPosition.x,
        ~blockRow,
        ~gridRow=currentGridRow,
      );

    Array.setUnsafe(newGrid, gridRowIndex, newGridRow);
  });

  newGrid;
};

let getStrokeIndexes = (grid: grid) => {
  let checkRow = gridRow =>
    gridRow->Array.reduce(true, (acc, elem) =>
      switch (acc, elem) {
      | (true, X) => true
      | (_, _) => false
      }
    );

  grid
  ->Array.reduceWithIndex(
      [],
      (acc, gridRow, i) => {
        let hasStroke = checkRow(gridRow);
        hasStroke ? [i, ...acc] : acc;
      },
    )
  ->List.toArray;
};

let removeFilledRows = (grid: grid, indexes) => {
  let indexesCount = Array.size(indexes);

  if (indexesCount == 0) {
    grid;
  } else {
    let hasIndex = i => Array.some(indexes, idx => idx == i);
    let emptyRow = Array.make(Array.size(Array.getUnsafe(grid, 0)), O);

    let filteredGrid = grid->Array.keepWithIndex((_el, i) => !hasIndex(i));

    filteredGrid->Array.concat(Array.make(indexesCount, emptyRow), _);
  };
};

let calcInitBlockPosition = (~block: block, ~gridWidth: int): blockPosition => {
  let blockWidth = getWidth(block);
  let blockHeight = getHeight(block);

  let posX = gridWidth / 2 - blockWidth / 2;
  let posY = Constants.Grid.hiddenRowsCount - blockHeight;

  let position = {x: posX, y: posY};
  position;
};

let genInitGridState = (~gridWidth: int, ~gridHeight: int): GridState.t => {
  let emptyRow = Array.make(gridWidth, O);
  let emptyGrid = Array.make(gridHeight, emptyRow);

  let firstBlock = Blocks.getRandomBlock();
  let initBlockPosition =
    calcInitBlockPosition(~block=firstBlock, ~gridWidth);

  {block: firstBlock, blockPosition: initBlockPosition, grid: emptyGrid};
};

let calcLevel = score => score / 500 + 1;

let calcNextStats = (~stats: GameStats.t, ~strokesCount: int): GameStats.t => {
  let {score, lines, level}: GameStats.t = stats;

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

let tick =
    (gridState: GridState.t, stats: GameStats.t, ~nextBlock=?, ())
    : TickOutput.t => {
  let {blockPosition, block, grid}: GridState.t = gridState;
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
    let nextGridState: GridState.t = {
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

    let nextGridState: GridState.t = {
      block: nextBlock,
      blockPosition:
        calcInitBlockPosition(~block=nextBlock, ~gridWidth=getWidth(grid)),
      grid: nextGridWithRemovedRows,
    };

    let canMapNextBlock =
      canMapBlock(nextGridState.blockPosition, nextBlock, nextGridState.grid);

    {
      gridState: nextGridState,
      stats: nextStats,
      gameOver: canMapNextBlock == false,
      nextBlockToShow: Blocks.getRandomBlock(),
    };
  };
};

let getNextPositionByDirection = (direction: Direction.t, currentPosition) => {
  let {x, y} = currentPosition;

  let nextPosition =
    switch (direction) {
    | Left => {x: x - 1, y}
    | Right => {x: x + 1, y}
    };

  nextPosition;
};

let getGridStateAfterMove =
    (direction: Direction.t, gridState: GridState.t): GridState.t => {
  let {blockPosition, block, grid}: GridState.t = gridState;

  let nextPosition = getNextPositionByDirection(direction, blockPosition);

  let canMap = canMapBlock(nextPosition, block, grid);

  canMap ? {...gridState, blockPosition: nextPosition} : gridState;
};

let rotate2dArr = (arr: array(array('a))) => {
  let initArr = Array.getUnsafe(arr, 0);

  let getColumn = (i, grid) =>
    grid->Array.map(row => Array.getUnsafe(row, i));

  initArr->Array.mapWithIndex((i, _row) => getColumn(i, arr));
};

let rotateClockwise = grid => grid->Array.reverse->rotate2dArr;

let getGridStateAfterRotate = (gridState: GridState.t): GridState.t => {
  let {blockPosition, block, grid}: GridState.t = gridState;
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
