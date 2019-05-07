open Types;

Random.init(int_of_float(Js.Date.now()));

let getRandomElemIndex = arr => Random.int(Array.length(arr));
let getRandomElemFromArr = arr => arr[getRandomElemIndex(arr)];

let mirror = block  => block -> Belt.Array.map(row => Belt.Array.reverse(row));

let blockT : block =
[|
[|X, X, X|],
[|O, X, O|],
|];

let blockBox : block =
[|
[|X, X|],
[|X, X|],
|];

let blockL : block =
[|
[|X, O|],
[|X, O|],
[|X, X|],
|];

let blockZ : block =
[|
[|O, X, X|],
[|X, X, O|],
|];

let blockI : block =
[|
[|X, X, X, X|]
|];

let blockColl = [|blockT, blockBox, blockI, blockL, mirror(blockL), blockZ, mirror(blockZ)|];

let getRandomBlock = () => getRandomElemFromArr(blockColl);
