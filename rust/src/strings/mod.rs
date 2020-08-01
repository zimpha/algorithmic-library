use std::cmp::min;

//pub mod suffix_array;
//pub mod suffix_automaton;

/// A border of a string is a prefix that is also a suffix of the string but not the whole string.
/// This function implements the KMP algorithm and returns a vector contains the border of each
/// prefix of `s`.
///
/// Time Complexity: `O(|s|)`.
pub fn border<T: PartialEq>(s: &[T]) -> Vec<usize> {
    let n = s.len();
    let mut border = vec![0; n];
    for i in 1..n {
        let mut j = border[i - 1];
        while j > 0 && s[j] != s[i] {
            j = border[j - 1];
        }
        border[i] = if s[j] == s[i] { j + 1 } else { j }
    }
    border
}

/// This function implements the Booth's Algorithm and returns the lexicographically minimal string
/// rotation.
///
/// For example, the lexicographically minimal rotation of `bbaaccaadd` would be `aaccaaddbb` and
/// the function call `minimal_rotation("bbaaccaadd")` will return `2` as result.
///
/// Time Complexity: `O(|s|)`.
pub fn minimal_rotation<T: Ord>(s: &[T]) -> usize {
    let n = s.len();
    let mut i = 0;
    let mut j = 1;
    loop {
        let mut k = 0;
        let mut ci = &s[i % n];
        let mut cj = &s[j % n];
        while k < n {
            ci = &s[(i + k) % n];
            cj = &s[(j + k) % n];
            if ci != cj {
                break;
            }
            k += 1
        }
        if k == n {
            return min(i, j);
        }
        if ci > cj {
            i += k + 1;
            i += (i == j) as usize;
        } else {
            j += k + 1;
            j += (i == j) as usize;
        }
    }
}

/// This function implements the Manacher's Algorithm and returns the length of the longest
/// palindrome of each center.
///
/// A string `s[1]s[2]...s[n]` is treated as `s'=s[1]-s[2]-...-s[n]` and the result `d[i]` is the
/// length of the longest palindrome whose cneter is `s'[i]`.
///
/// For example `babbaa` is treated as `b-a-b-b-a-a`, the result `d[]` will be `[1, 0, 3, 0, 1, 4,
/// 1, 0, 1, 2, 1]`.
///
/// Time Complexity: `O(|s|)`.
pub fn manacher<T: PartialEq>(s: &[T]) -> Vec<usize> {
    let n = s.len() * 2;
    let mut d = vec![0; n];
    let mut i = 0;
    let mut j = 0;
    while i < n {
        while i >= j && i + j + 1 < n && s[(i - j) >> 1] == s[(i + j + 1) >> 1] {
            j += 1;
        }
        d[i] = j;
        let mut k = 1;
        while i >= k && d[i] >= k && d[i - k] != d[i] - k {
            d[i + k] = min(d[i - k], d[i] - k);
            k += 1;
        }
        i += k;
        j = if j >= k { j - k } else { 0 };
    }
    d
}
