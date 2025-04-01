export function _AND_(a, b) { return a && b; }
export function _OR_(a, b) { return a || b; }
export function _equals_(a, b) { return a === b; }
export function _mapAccess_(map, key) { return map[key]; } // Correcting `mapAcess`
export function _inMapVerify_(map, key) { return key in map; }
export function _inList_(list, item) { return list.includes(item); }
export function _getRoles_(user) { return user.roles; }
export function _getId_(user) { return user.id; }
export function _getStudent_(id, token) { console.log("Fetching student:", id); }
