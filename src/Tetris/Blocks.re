open Types;

Random.init(int_of_float(Js.Date.now()));

let getRandomElemIndex = arr => Random.int(Array.length(arr));
let getRandomElemFromArr = arr => arr[getRandomElemIndex(arr)];

let mirror = block => block |> Array.map(row => Belt.Array.reverse(row));

let blockT =
[|
[|X, X, X|],
[|O, X, O|],
|];

let blockBox =
[|
[|X, X|],
[|X, X|],
|];

let blockL =
[|
[|X, O|],
[|X, O|],
[|X, X|],
|];

let blockZ =
[|
[|O, X, X|],
[|X, X, O|],
|];

let blockI =
[|
[|X, X, X, X|]
|];

let blockColl = [|blockT, blockBox, blockI, blockL, mirror(blockL), blockZ, mirror(blockZ)|];

let getRandomBlock = () => getRandomElemFromArr(blockColl);
