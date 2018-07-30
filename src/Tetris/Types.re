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
