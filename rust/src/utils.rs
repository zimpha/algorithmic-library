use std::usize;

pub const NULL_INDEX: usize = usize::MAX;

pub fn group_by<I, T>(xs: I) -> Vec<(T, usize)>
where
    I: Iterator<Item = T>,
    T: Eq,
{
    let mut groups = Vec::<(T, usize)>::new();
    for item in xs {
        let last = groups.last_mut();
        if let Some((_, cnt)) = last.filter(|(k, _)| k == &item) {
            *cnt += 1;
        } else {
            groups.push((item, 1));
        }
    }
    groups
}

pub fn unique<I, T>(xs: I) -> Vec<T>
where
    I: Iterator<Item = T>,
    T: Eq,
{
    let mut vs = Vec::<T>::new();
    for item in xs {
        if vs.is_empty() || vs.last().unwrap() != &item {
            vs.push(item)
        }
    }
    vs
}
