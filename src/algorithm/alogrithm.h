#pragma once

template <typename T> class Alogrithm {
protected:
	bool bubble(Rank lo, Rank hi);
	void bubblesort(Rank lo, Rank hi);
	void selectionsort(Rank lo, Rank hi);
	void merge(Rank lo, Rank hi);
	void mergesort(Rank lo, Rank hi);
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);
};
