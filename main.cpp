#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>


const int MaxStr = 300;
const int MaxSize = 7000;
const int MaxArr = 100000;

void set_iter(int arr[], int num_elem);

void swap(int a1, int a2);

int readTextRows(int positions[], FILE* text);
char* inputText(FILE* text, int positions[], int num_rows);

void writeText(FILE* fout, int ptrarr[], int positions[], int num_elem, char* text);

void heapify(int ptrarr[], char* text, int positions[], int num_elem, int v);

void build_heap(int ptrarr[], char* text, int positions[], int num_elem);

void heap_sort(int ptrarr[], char* text, int positions[], int num_elem);


bool compareStr(int ptr1, int ptr2, int positions[], char* text);
bool backCompareStr(int ptr1, int ptr2, int positions[], char* text);

void quickSort(int ptrarr[], int positions[], char* text, int left, int right, int num_elem, bool (*compare)(int ptr1, int ptr2, int positions[], char* text));

int main()
{
  FILE * OneginF      = fopen("origOnegin", "r");
  FILE * Encyclopedia = fopen("encyclOnegin", "w");

  int positions[MaxSize] = {};

  int num_rows = readTextRows(positions, OneginF);
  fseek(OneginF, 0, SEEK_SET);

  char* text = inputText(OneginF, positions, num_rows);

  int* ptrarr = (int*)(calloc(num_rows, sizeof(*ptrarr)));
  int* ptrarrSort = (int*)(calloc(num_rows, sizeof(*ptrarr)));
  int* ptrarrRhyme = (int*)(calloc(num_rows, sizeof(*ptrarr)));
  set_iter(ptrarr, num_rows);
  set_iter(ptrarrSort, num_rows);
  set_iter(ptrarrRhyme, num_rows);

  printf("%i:: Num rows = %i\n", __LINE__, num_rows);
  // heap_sort(ptrarr, text, positions, num_rows);

  quickSort(ptrarrSort, positions, text, 0, num_rows - 1, num_rows, compareStr);

  writeText(Encyclopedia, ptrarrSort, positions, num_rows, text);

  for (int ch = 0; ch < 20; ch ++)
  {
    fprintf(Encyclopedia, "-");
  }
  fprintf(Encyclopedia, "\n");

  quickSort(ptrarrRhyme, positions, text, 0, num_rows - 1, num_rows, backCompareStr);

  writeText(Encyclopedia, ptrarrRhyme, positions, num_rows, text);

  writeText(Encyclopedia, ptrarr, positions, num_rows, text);

  free(text);
  free(ptrarr);
  free(ptrarrSort);
  free(ptrarrRhyme);

  fclose(OneginF);
  fclose(Encyclopedia);
}

void set_iter(int array[], int num_elem)
{
  for (int it = 0; it < num_elem; it ++)
  {
    array[it] = it;
  }
}

bool is_letter(char chr)
{
  if (('a' <= chr && chr <= 'z') || ('A' <= chr && chr <= 'Z'))
  {
    return true;
  }
  return false;
}

char toLower(char chr)
{
  // if (!is_letter(chr))
  // {
  //   printf("%i:: Chr = %c\n", __LINE__, chr);
  // }
  assert(is_letter(chr));

  return ('a' <= chr && chr <= 'z') ? chr : chr + 'a' - 'A';
}

void swap(int* a1, int* a2)
{
  int temp = (*a1);
  (*a1) = (*a2);
  (*a2) = temp;
}
// -- Comparators -----------------------------------------------------

bool compareStr(int ptr1, int ptr2, int positions[], char* text)
{
  bool have_letter1 = false;
  bool have_letter2 = false;
  for (int chr1 = positions[ptr1], chr2 = positions[ptr2]; chr1 < positions[ptr1 + 1] && chr2 < positions[ptr2 + 1]; chr1 ++, chr2 ++)
  {
    while (chr1 < positions[ptr1 + 1] && !is_letter(text[chr1])) chr1 ++;
    while (chr2 < positions[ptr2 + 1] && !is_letter(text[chr2])) chr2 ++;

    if (chr1 == positions[ptr1 + 1])
    {
      if (have_letter1)
      {
        if (have_letter2)
        {
          break;
        }
        return false;
      }
      return true;
    }
    if (chr2 == positions[ptr2 + 1])
    {
      if (have_letter2)
      {
        if (have_letter1)
        {
          break;
        }
        return true;
      }
      return false;
    }

    have_letter1 = true;
    have_letter2 = true;

    if (text[chr1] == text[chr2])
    {
      chr1 ++;
      chr2 ++;
      continue;
    }
    return (toLower(text[chr1]) > toLower(text[chr2]));
  }
  return ((positions[ptr1 + 1] - positions[ptr1]) > (positions[ptr2 + 1] - positions[ptr2]));
}

bool backCompareStr(int ptr1, int ptr2, int positions[], char* text)
{
  bool have_letter1 = false;
  bool have_letter2 = false;
  for (int chr1 = positions[ptr1 + 1] - 1, chr2 = positions[ptr2 + 1] - 1; chr1 >= positions[ptr1] && chr2 >= positions[ptr2]; chr1 --, chr2 --)
  {
    while (positions[ptr1] <= chr1 && !is_letter(text[chr1])) chr1 --;
    while (positions[ptr2] <= chr2 && !is_letter(text[chr2])) chr2 --;

    if (chr1 == positions[ptr1] - 1)
    {
      if (have_letter1)
      {
        if (have_letter2)
        {
          goto exit;
        }
        return false;
      }
      return true;
    }
    if (chr2 == positions[ptr2] - 1)
    {
      if (have_letter2)
      {
        if (have_letter1)
        {
          goto exit;
        }
        return true;
      }
      return false;
    }

    have_letter1 = true;
    have_letter2 = true;

    if (text[chr1] == text[chr2])
    {
      chr1 --;
      chr2 --;
      continue;
    }
      return (toLower(text[chr1]) > toLower(text[chr2]));
  }
  exit:
    return ((positions[ptr1 + 1] - positions[ptr1]) > (positions[ptr2 + 1] - positions[ptr2]));
}




// --------------------------------------------------------------------

// -- Reading file ----------------------------------------------------

int readTextRows(int positions[], FILE* text)
{
  int cnt_rows = 1;

  char* str = (char*)(calloc(MaxStr, sizeof(*str)));

  int cur_pos = 1;

  while (fgets(str, MaxStr, text))
  {
    positions[cur_pos] = strlen(str) + positions[cur_pos - 1];
    cur_pos ++;
    cnt_rows ++;
  }

  free(str);

  return cnt_rows;
}

char* inputText(FILE* text, int positions[], int num_rows)
{
  char* outtext = (char *)(calloc(positions[num_rows - 1] + 1, sizeof(*outtext)));

  for (int row = 1; row < num_rows; row ++)
  {
      char* str = (char*)(calloc(positions[row] - positions[row - 1] + 1, sizeof(*str)));

    fgets(str, MaxStr, text);

    strcat(outtext, str);

    free(str);
  }

  return outtext;
}

// --------------------------------------------------------------------

void writeText(FILE* fout, int ptrarr[], int positions[], int num_elem, char* text)
{
  for (int row = 0; row < num_elem; row ++)
  {
    // printf("%i:: Row no. %i" "; Ptrarr[%i] = %i; Pos = %i; -> %i" "\n", __LINE__, row, row, ptrarr[row], positions[ptrarr[row]], positions[ptrarr[row] + 1]);
    for (int chr = positions[ptrarr[row]]; chr < positions[ptrarr[row] + 1] - 2; chr ++)
    {
      fprintf(fout, "%c", text[chr]);
    }
    fprintf(fout, "\n");
  }
}

// -- Sorting ---------------------------------------------------------


void heapify(int ptrarr[], char* text, int positions[], int num_elem, int v)
{
    int largest = v;
    int left_child = 2 * v + 1;
    int right_child = 2 * v + 2;

    if (left_child < num_elem && compareStr(ptrarr[left_child], ptrarr[largest], positions, text))
        largest = left_child;

    if (right_child < num_elem && compareStr(ptrarr[right_child], ptrarr[largest], positions, text))
        largest = right_child;

    if (largest != v)
    {
        swap(&(ptrarr[v]), &(ptrarr[largest]));

        heapify(ptrarr, text, positions, num_elem, largest);
    }
}


void build_heap(int ptrarr[], char* text, int positions[], int num_elem)
{
  int ind = num_elem / 2 - 1;

  for (int i = ind; i >= 0; i --)
  {
    heapify(ptrarr, text, positions, i, 0);
  }

//    for (int i = 0; i < num_elem; i ++) {
//        printf("%i ", ptrarr[i]);
//        if (i == num_elem - 1) printf("\n");
//    }

}

void heap_sort(int ptrarr[], char* text, int positions[], int num_elem) {
    build_heap(ptrarr,text, positions, num_elem);

    for (int i = num_elem - 1; i >= 0; i --) {
      swap(&(ptrarr[0]), &(ptrarr[i]));

      heapify(ptrarr, text, positions, i, 0);
    }
}




// --------------------------------------------------------------------

// -- QSort -----------------------------------------------------------

void quickSort(int ptrarr[], int positions[], char* text, int left, int right, int num_elem, bool (*compare)(int ptr1, int ptr2, int positions[], char* text))
{
  assert(left <= right);
  assert(right <= num_elem);
  assert(num_elem <= MaxArr);

  int pivot = ptrarr[left];
  int l_wall = left;
  int r_wall = right;

  while (left < right)
  {
    while (left < right && compare(ptrarr[right], pivot, positions, text))
    {
      right --;
    }
    if (left != right)
    {
      ptrarr[left ++] = ptrarr[right];
      // swap(&(array[left++]), &(array[right]));
    }

    while (left < right && compare(pivot, ptrarr[left], positions, text))
    {
      left ++;
    }
    if (left != right)
    {
      ptrarr[right --] = ptrarr[left];
      // swap(&(array[right --]), &(array[left]));
    }
  }

  ptrarr[left] = pivot;
  pivot = left;
  left = l_wall;
  right = r_wall;
  if (left < pivot)
    quickSort(ptrarr, positions, text, left, pivot - 1, num_elem, compare);
  if (right > pivot)
    quickSort(ptrarr, positions, text, pivot + 1, right, num_elem, compare);
}



// --------------------------------------------------------------------
