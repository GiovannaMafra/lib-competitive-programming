
// ===================== CLOSEST PAIR =====================

// Indices (i < j) of the closest pair of points in pts, by Euclidean distance.
// O(n log n). Assumes pts.size() >= 2.
pair<int,int> closest_pair(vector<pt>& pts) {
	int n = pts.size();
	ld best = dist2(pts[0], pts[1]);
	pair<int,int> ans(0, 1);

	vector<int> ord(n);
	iota(ord.begin(), ord.end(), 0);
	sort(ord.begin(), ord.end(), [&](int a, int b) { return pts[a].x < pts[b].x; });

	set<pair<ld,int>> active; // points inside the current x-window, sorted by y
	for (int l = 0, r = 0; r < n; r++) {
		while (sq(pts[ord[r]].x - pts[ord[l]].x) > best) {
			active.erase({pts[ord[l]].y, ord[l]});
			l++;
		}
		ld delta = sqrt(best) + eps;
		auto itl = active.lower_bound({pts[ord[r]].y - delta, -1});
		auto itr = active.upper_bound({pts[ord[r]].y + delta, n + 1});
		for (auto it = itl; it != itr; it++) {
			ld cur = dist2(pts[ord[r]], pts[it->second]);
			if (cur < best) {
				best = cur;
				ans = {ord[r], it->second};
			}
		}
		active.insert({pts[ord[r]].y, ord[r]});
	}
	if (ans.first > ans.second) swap(ans.first, ans.second);
	return ans;
}

/*LATEX_DESC_BEGIN***************************

   CLOSEST PAIR

   closest_pair -> par de pontos com a menor distancia euclidiana. O(n log n)

*****************************LATEX_DESC_END*/