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
  blockRow
  |> Js.Array.reducei(
       (acc, blockCell, i) =>
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
         },
       true,
     );

let canMapBlock = (position: blockPosition, block: block, grid: grid) => {
  let isValidPos = isValidPosition(position, block, grid);

  isValidPos
  && block
  |> Js.Array.reducei(
       (acc, blockRow, i) =>
         switch (acc) {
         | false => false
         | true =>
           let gridRow = grid[position.y + i];
           gridRow |> canMapRow(position.x, blockRow);
         },
       true,
     );
};

let mapBlockRowToGridRow = (posX: int, blockRow: row, gridRow: row) => {
  let newGridRow = Array.copy(gridRow);

  blockRow
  |> Array.iteri((i, blockCell) =>
       switch (blockCell) {
       | O => ()
       | X => newGridRow[posX + i] = blockCell
       }
     );

  newGridRow;
};

let mapBlockToGrid = (position: blockPosition, block: block, grid: grid) => {
  let newGrid = Array.copy(grid);

  block
  |> Array.iteri((i, blockRow) => {
       let gridRowIndex = position.y + i;
       newGrid[gridRowIndex] =
         mapBlockRowToGridRow(position.x, blockRow, grid[gridRowIndex]);
     });

  newGrid;
};

let mapBlockToGridOk = ({blockPosition, block, grid}: gridState) => {
  let newGrid = Array.copy(grid);

  block
  |> Array.iteri((i, blockRow) => {
       let gridRowIndex = blockPosition.y + i;
       newGrid[gridRowIndex] =
         mapBlockRowToGridRow(blockPosition.x, blockRow, grid[gridRowIndex]);
     });

  newGrid;
};

let getStrokeIndexes = (grid: grid) => {
  let checkRow = gridRow =>
    gridRow
    |> Array.fold_left(
         (acc, elem) =>
           switch (acc, elem) {
           | (true, X) => true
           | (_, _) => false
           },
         true,
       );

  grid
  |> Js.Array.reducei(
       (acc, gridRow, i) => {
         let hasStroke = checkRow(gridRow);
         hasStroke ? [i, ...acc] : acc;
       },
       [],
     )
  |> Array.of_list;
};

let removeFilledRows = (grid: grid) => {
  let indexes = getStrokeIndexes(grid);
  let indexesCount = Array.length(indexes);

  if (indexesCount == 0) {
    grid;
  } else {
    let hasIndex = i => Js.Array.includes(i, indexes);
    let emptyRow = Array.make(Array.length(grid[0]), O);

    let filteredGrid = grid |> Js.Array.filteri((_el, i) => ! hasIndex(i));

    filteredGrid |> Array.append(Array.make(indexesCount, emptyRow));
  };
};

let genInitBlockPosition = (block: block, grid: grid) : blockPosition => {
  let blockWidth = getWidth(block);
  let blockHeight = getHeight(block);
  let gridWidth = getWidth(grid);

  let posX = Random.int(gridWidth - blockWidth);
  let posY = 3 - blockHeight;

  let position = {x: posX, y: posY};
  position;
};

let tick = ({blockPosition, block, grid}: gridState) => {
  let {x, y} = blockPosition;
  let nextPosition = {x, y: y + 1};

  let canMap = canMapBlock(nextPosition, block, grid);

  if (canMap) {
    {block, blockPosition: nextPosition, grid};
  } else {
    let nextGrid =
      mapBlockToGridOk({blockPosition, block, grid}) |> removeFilledRows;

    let nextBlock = Blocks.getRandomBlock();

    let nextState = {
      block: nextBlock,
      blockPosition: genInitBlockPosition(nextBlock, grid),
      grid: nextGrid,
    };
    nextState;
  };
};

let isGameOver = ({blockPosition, block, grid}: gridState) => {
  let canMap = canMapBlock(blockPosition, block, grid);
  let {y} = blockPosition;
  let blockHeight = getHeight(block);

  y == (3 - blockHeight) && ! canMap;
};

let getNextPositionByDirection = (direction, currentPosition) => {
  let {x, y} = currentPosition;

  let nextPosition =
    switch (direction) {
    | Left => {x: x - 1, y}
    | Right => {x: x + 1, y}
    | Down => {x, y: y + 1}
    | _ => currentPosition
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

  let getColumn = (i, grid) => grid |> Array.map(row => row[i]);

  initArr |> Array.mapi((i, _row) => getColumn(i, arr));
};

let reverseArr = Belt.Array.reverse;

let rotateClockwise = grid => grid |> reverseArr |> rotate2dArr;

let getGridStateAfterRotate = gridState => {
  let {blockPosition, block, grid} = gridState;
  let rotatedBlock = rotateClockwise(block);

  let canMap = canMapBlock(blockPosition, rotatedBlock, grid);

  canMap ? {...gridState, block: rotatedBlock} : gridState;
};
