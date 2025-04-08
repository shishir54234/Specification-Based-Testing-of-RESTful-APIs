// testUtils.js

/**
 * Checks an assumption and throws an error if it fails.
 * @param {boolean} condition - The assumption to evaluate.
 * @throws {Error} Throws an error if the condition is false.
 */
export function assume(condition) {
    if (!condition) {
        throw new Error("Assumption failed");
    }
}

/**
 * Asserts that a condition is true.
 * Throws an error if the condition is false,
 * and logs "Test passed" if it is true.
 * @param {boolean} condition - The condition to assert.
 * @throws {Error} Throws an error if the condition is false.
 */
export function assert(condition) {
    if (!condition) {
        throw new Error("Assertion failed");
    } else {
        console.log("Test passed");
    }
}
