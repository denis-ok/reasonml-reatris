open Types;
open Belt;

let mirror = block => block->Array.map(row => Array.reverse(row));

let blockT: block = [|[|X, X, X|], [|O, X, O|]|];

let blockBox: block = [|[|X, X|], [|X, X|]|];

let blockL: block = [|[|X, O|], [|X, O|], [|X, X|]|];

let blockZ: block = [|[|O, X, X|], [|X, X, O|]|];

let blockI: block = [|[|X, X, X, X|]|];

let blocks = [|
  blockT,
  blockBox,
  blockI,
  blockL,
  mirror(blockL),
  blockZ,
  mirror(blockZ),
|];

let getRandomBlock = () => Array.shuffle(blocks)->Array.getUnsafe(0);
