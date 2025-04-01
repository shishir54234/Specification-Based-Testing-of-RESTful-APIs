
function input(symbolholder) {
    return S$.symbol(symbolholder, 'symbolic_variable'); 
}

function _in(key,collection){
    return collection.has(key)
}
function dom(map) {
    return new Set(map.keys()); // Returns a Set of keys
}

function not_in(key, collection) {
    return !collection.has(key);
}

// Mock function to assume a condition
function assume(condition) {
    return S$.assume(condition);
}


// Mock assertion function
function assert(condition) {
    if (!condition) {
        throw new Error("Assertion failed");
    }
}


// Simulates a signup function
function signup(username, password) {
    console.log(`Signed up with username: ${username} and password: ${password}`);
}

