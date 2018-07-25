type cell =
  | O
  | X;

type position = {
  x: int,
  y: int,
};

type row = array(cell);

let emptyRowT: row = Array.make(10, O);
let emptyGridT = Array.make(10, emptyRowT);

let blockT = [|[|O, X, O|], [|O, X, O|], [|X, X, X|]|];

let emptyRow = Array.make(10, 0);
let emptyGrid = Array.make(10, emptyRow);
let block1 = [|[|0, 1, 0|], [|0, 1, 0|], [|1, 1, 1|]|];

let mapElementsToArray = (position: int, elements: array('a), array: array('a)) => {
  let elementsCount = Array.length(elements);
  array |> Array.mapi(
    (i, elem) =>
      if (i >= position && i < position + elementsCount) {
        let newElem = elements[i - position];
        newElem;
      } else {
        elem;
      }
  );
};

let mapBlockRowToGridRow = (position, blockRow, gridRow) =>
  mapElementsToArray(position, blockRow, gridRow);

let mapBlockToGrid = (posX: int, posY: int, block, grid) => {
  let blockSize = Array.length(block);
  grid
  |> Array.mapi((gridPosY, gridRow) =>
       if (gridPosY >= posY && gridPosY < posY + blockSize) {
         let newRow = mapBlockRowToGridRow(posX, block[gridPosY - posY], gridRow);
         newRow;
       } else {
         gridRow;
       }
     );
};

/* let getGridSliceForBlock = (posY, block, grid) => Array.sub(grid, posY, Array.length(block)); */

let mapBlockToGridNew = (posX: int, posY: int, block, grid) => {
  let gridRowsForMap = Array.sub(grid, posY, Array.length(block));

  let mappedBlock =
    block
    |> Array.mapi((i, blockRow) =>
         mapBlockRowToGridRow(posX, blockRow, gridRowsForMap[i])
       );

  mapElementsToArray(posY, mappedBlock, grid);
};

let isValidPosition = (posX, posY, block, grid) => {
  let blockHeight = Array.length(block);
  let blockWidth = Array.length(block[0]);

  let gridHeight = Array.length(grid);
  let gridWidth = Array.length(grid[0]);

  let isValidX = posX >= 0 && posX <= gridWidth - blockWidth;
  let isValidY = posY >= 0 && posY <= gridHeight - blockHeight;

  isValidX && isValidY;
};

let canMapRow = (posX, blockRow, gridRow) =>
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

let canMapBlock = (posX, posY, block, grid) =>
  block
  |> Js.Array.reducei(
       (acc, blockRow, i) =>
         switch (acc) {
         | false => false
         | true =>
           let gridRow = grid[posY + i];
           gridRow |> canMapRow(posX, blockRow);
         },
       true,
     );

let mapBlockRowToGridRowImp = (posX, blockRow, gridRow) => {
  let newGridRow = Array.copy(gridRow);

  blockRow |> Array.iteri((i, blockCell) => {
    switch blockCell {
    | O => ()
    | X => newGridRow[posX + i] = blockCell
    };
  });

  newGridRow;
};

let mapBlockToGridImp = (posX, posY, block, grid) => {
  let newGrid = Array.copy(grid);

  block |> Array.iteri((i, blockRow) => {
    let gridRowIndex = posY + i;
    newGrid[gridRowIndex] = mapBlockRowToGridRowImp(posX, blockRow, grid[gridRowIndex])
  });

  newGrid;
};

let getStrokeIndexes = (grid) => {
  let checkRow = gridRow => {
    gridRow |> Array.fold_left((acc, elem) =>
    switch (acc, elem) {
    | (true, X) => true
    | (_, _) => false
    }, true);
  };

  grid
  |> Js.Array.reducei(
       (acc, gridRow, i) => {
         let hasStroke = checkRow(gridRow);
         switch hasStroke {
         | true => [i, ...acc]
         | false => acc
         };
       },
       [],
     )
  |> Array.of_list;
};

let getGridSliceForBlock = (posY, block, grid) =>
  Array.sub(grid, posY, Array.length(block));

let removeFilledRows = (grid) => {
  let indexes = getStrokeIndexes(grid);
  let indexesCount = Array.length(indexes);

  if (indexesCount == 0) {
    grid
  } else {
    let hasIndex = i => Js.Array.includes(i, indexes);
    let emptyRow = Array.make(Array.length(grid[0]), O);

    let filteredGrid = grid |> Js.Array.filteri((_el, i) => !hasIndex(i));

    filteredGrid |> Array.append(Array.make(indexesCount, emptyRow));
  }
};
