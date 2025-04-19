const S$ = require('S$');
let U;
let T;

// let prodid;
// usr1==usr2
// let usr1,usr2;
let ProductIdMap;
let Wishlist;
let username;
let password;
U = new Map([[alice, "alicepass"], [bob, "bob123"], [charlie, "qwerty"]]);
T = new Map([]);
ProductIdMap = new Map([[prod1, ["Running Shoe", "Lightweight runner", 120, "seller123", "Footwear"]], [prod2, ["Leather Wallet", "Genuine leather", 45, "seller456", "Accessories"]]]);
Wishlist = new Map([[alice, ["prod1", "prod2"]], [bob, ["prod3"]]]);
username = input();
password = input();
assume(and_operator(equals(password, mapped_value(U, username)), in_dom(T, token)));
login_success(username, password);
assert(equals(mapped_value(T, token), username));
assume(equals(username, mapped_value(T, token)));
getProducts(username);
assert(true());
assume(and_operator(equals(u, mapped_value(T, token)), in(productID, dom(ProductIdMap))));
viewProduct(productID);
assert(true());
assume(and_operator(equals(username, mapped_value(T, token)), in(productID, dom(ProductIdMap))));
addToWishlist(productID, username);
assert(in(productID, dom(getMapAtMatch(Wishlist, username))));
assume(equals(username, mapped_value(T, token)));
viewWishlist(username);
assert(true([["prodID", "prodAtrib"]]));


// function mockedAddToWishlist(productID, username) {
//     // tupleofprods=[[String,String,int]];
//     // product[0]
//     return tupleofprods;
// }

const tupleofprods,respcode
function mockedAddToWishlist(productID, username){
    tupleofprods= S$.symbol("symval",[["string","string",1]]);
}
assume();
mockedAddToWishlist(productID, username);
assert(_in(productID, dom(mapped_value(Wishlist, username))));
// assert()
// Wishlist[username] is tupleofprods;