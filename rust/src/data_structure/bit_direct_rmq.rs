use std::cmp::min;

pub struct BitDirectRMQ {
    lookup: Vec<usize>,
    log: Vec<usize>,
    table: Vec<usize>,
    blocks: usize,
}

impl BitDirectRMQ {
    const BLOCK_SIZE: usize = 32;

    pub fn with_capacity(capacity: usize) -> Self {
        let blocks = (capacity + Self::BLOCK_SIZE - 1) / Self::BLOCK_SIZE;
        let mut height = 0;
        while (1 << height) < blocks {
            height += 1;
        }
        let mut log = vec![0; blocks + 1];
        for i in 2..blocks + 1 {
            log[i] = log[i >> 1] + 1;
        }

        BitDirectRMQ {
            lookup: vec![0; capacity],
            log,
            table: Vec::with_capacity(blocks * (height + 1)),
            blocks: 0,
        }
    }

    pub fn build<T: Ord>(&mut self, a: *const T, n: usize) {
        self.blocks = (n + Self::BLOCK_SIZE - 1) / Self::BLOCK_SIZE;
        self.build_sparse_table(a, n);
        self.build_inner_blocks(a, n);
    }

    /// return the index of the minimum value in [l, r] in O(1)
    pub fn query<T: Ord>(&self, a: *const T, l: usize, r: usize) -> usize {
        let x = l / Self::BLOCK_SIZE;
        let y = r / Self::BLOCK_SIZE;
        if x == y {
            return self.query_lookup(l, r);
        }
        let mut ret = if x + 1 <= y - 1 {
            self.query_st(a, x + 1, y - 1)
        } else {
            l
        };
        ret = Self::min_index(
            a,
            ret,
            self.query_lookup(l, x * Self::BLOCK_SIZE + Self::BLOCK_SIZE - 1),
        );
        Self::min_index(a, ret, self.query_lookup(y * Self::BLOCK_SIZE, r))
    }

    fn query_st<T: Ord>(&self, a: *const T, x: usize, y: usize) -> usize {
        let z = self.log[y - x + 1];
        unsafe {
            return Self::min_index(
                a,
                *self.table.get_unchecked(x + z * self.blocks),
                *self.table.get_unchecked(y - (1 << z) + 1 + z * self.blocks),
            );
        }
    }

    fn query_lookup(&self, x: usize, y: usize) -> usize {
        unsafe {
            return x
                + (*self.lookup.get_unchecked(y) >> (x % Self::BLOCK_SIZE)).trailing_zeros()
                    as usize;
        }
    }

    fn min_index<T: Ord>(a: *const T, x: usize, y: usize) -> usize {
        unsafe {
            if *a.add(x) < *a.add(y) {
                x
            } else {
                y
            }
        }
    }

    fn build_sparse_table<T: Ord>(&mut self, a: *const T, n: usize) {
        let mut height = 0;
        while (1 << height) < self.blocks {
            height += 1;
        }
        self.table
            .resize_with(self.blocks * (height + 1), Default::default);
        let mut ptr = self.table.as_mut_ptr();
        for idx in 0..self.blocks {
            let mut best_idx = idx * Self::BLOCK_SIZE;
            let bound = min(n, (idx + 1) * Self::BLOCK_SIZE);
            for j in best_idx + 1..bound {
                best_idx = Self::min_index(a, best_idx, j);
            }
            unsafe {
                *ptr.add(idx) = best_idx;
            }
        }
        for i in 0..height {
            let t = 1 << i;
            unsafe {
                ptr = ptr.add(self.blocks);
                ptr.copy_from_nonoverlapping(ptr.sub(self.blocks), self.blocks);
                for i in 0..self.blocks - t {
                    *ptr.add(i) = Self::min_index(a, *ptr.add(i), *ptr.add(i + t));
                }
            }
        }
    }

    fn build_inner_blocks<T: Ord>(&mut self, a: *const T, n: usize) {
        let ptr = self.lookup.as_mut_ptr();
        unsafe {
            ptr.write_bytes(0, n);
        }
        let mut stk = vec![0; Self::BLOCK_SIZE];
        for i in 0..self.blocks {
            let l = i * Self::BLOCK_SIZE;
            let r = min(n, l + Self::BLOCK_SIZE);
            let mut state = 0;
            let mut top = 0;
            for j in l..r {
                unsafe {
                    while top > 0 && *a.add(j) < *a.add(*stk.get_unchecked(top - 1)) {
                        state ^= 1 << (*stk.get_unchecked(top - 1) - l);
                        top -= 1;
                    }
                    *stk.get_unchecked_mut(top) = j;
                    state ^= 1 << (j - l);
                    top += 1;
                    *ptr.add(j) = state;
                }
            }
        }
    }
}
