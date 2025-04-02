export function _AND_(a, b) {
    return a && b;
}

export function _OR_(a, b) {
    return a || b;
}

export function _equals_(a, b) {
    return a === b;
}

export function _mapAccess_(map, key) {
    return map[key];
}

export function _inMapVerify_(map, key) {
    return key in map;
}

export function _inList_(list, item) {
    return list.includes(item);
}

export function _getRoles_(user) {
    return user.roles;
}

export function _getId_(user) {
    return user.id;
}

export function _getStudent_(id, token) {
    console.log("Fetching student:", id);
}

export async function fetchData(baseUrl, params = {}) {
    try {
        // Convert params object into query string
        const queryString = new URLSearchParams(params).toString();
        const url = queryString ? `${baseUrl}?${queryString}` : baseUrl;

        console.log("Request URL:", url); // For debugging

        const response = await fetch(url);
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
        const data = await response.json();
        console.log("Response Data:", data);
    } catch (error) {
        console.error("Error fetching data:", error);
    }
}

// // Example usage with multiple parameters
// fetchData("https://jsonplaceholder.typicode.com/todos", {
//     userId: 1,
//     completed: false,
//     limit: 5
// });

