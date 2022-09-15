#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "generic/THLab.c"
#else

void THLab_(numel)(THTensor *t, int *n_)
{
  *n_ = THTensor_(nElement)(t);
}

void THLab_(max)(THTensor *t, int dimension, THTensor *values_, THLongTensor *indices_)
{
  THLongStorage *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");

  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(values_, dim);
  THLongTensor_resize(indices_, dim);
  THLongStorage_free(dim);

  TH_TENSOR_DIM_APPLY3(real, Real, t, real, Real, values_, long, Long, indices_, dimension,
                       long theIndex = 0;
                       real theMax = t_data[0];
                       for(i = 1; i < t_size; i++)
                       {
                         if(t_data[i*t_stride] > theMax)
                         {
                           theIndex = i;
                           theMax = t_data[i*t_stride];
                         }
                       }
                       *indices__data = theIndex;
                       *values__data = theMax;);  

}

void THLab_(min)(THTensor *t, int dimension, THTensor *values_, THLongTensor *indices_)
{
  THLongStorage *dim;
  long i;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");

  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(values_, dim);
  THLongTensor_resize(indices_, dim);
  THLongStorage_free(dim);

  TH_TENSOR_DIM_APPLY3(real, Real, t, real, Real, values_, long, Long, indices_, dimension,
                       long theIndex = 0;
                       real theMin = t_data[0];
                       for(i = 1; i < t_size; i++)
                       {
                         if(t_data[i*t_stride] < theMin)
                         {
                           theIndex = i;
                           theMin = t_data[i*t_stride];
                         }
                       }
                       *indices__data = theIndex;
                       *values__data = theMin;);  

}


void THLab_(sum)(THTensor *t, int dimension, THTensor *r_)
{
  THLongStorage *dim;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(r_, dim);
  THLongStorage_free(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                             sum += t_data[i*t_stride];
                           *r__data = sum;);
}

void THLab_(prod)(THTensor *t, int dimension, THTensor *r_)
{
  THLongStorage *dim;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");

  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(r_, dim);
  THLongStorage_free(dim);
  
  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real prod = 1;
                           long i;
                           for(i = 0; i < t_size; i++)
                             prod *= t_data[i*t_stride];
                           *r__data = prod;);

}

void THLab_(cumsum)(THTensor *t, int dimension, THTensor *r_)
{
  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  THTensor_(resizeAs)(r_, t);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real cumsum = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             cumsum += t_data[i*t_stride];
                             r__data[i*r__stride] = cumsum;
                           });
}

void THLab_(cumprod)(THTensor *t, int dimension, THTensor *r_)
{
  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "dimension out of range");
  
  THTensor_(resizeAs)(r_, t);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real cumprod = 1;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             cumprod *= t_data[i*t_stride];
                             r__data[i*r__stride] = cumprod;
                           });
}

void THLab_(trace)(THTensor *t, real *trace_)
{
  real *t_data = THTensor_(data)(t);
  real sum = 0;
  long i = 0;
  long t_stride, t_diag_size;

  THArgCheck(THTensor_(nDimension)(t) == 2, 1, "not a matrix");

  t_stride = THTensor_(stride)(t, 1);
  t_diag_size = THMin(THTensor_(size)(t, 0), THTensor_(size)(t, 1));
  while(i < t_diag_size)
  {
    sum += t_data[i*(t_stride+1)];
    i++;
  }

  *trace_ = sum;
}

void THLab_(cross)(THTensor *a, THTensor *b, int dimension, THTensor *r_)
{
  int i;

  if(THTensor_(nDimension)(a) != THTensor_(nDimension)(b))
    THError("inconsitent tensor sizes");
  
  for(i = 0; i < THTensor_(nDimension)(a); i++)
  {
    if(THTensor_(size)(a, i) != THTensor_(size)(b, i))
      THError("inconsistent tensor sizes");
  }
  
  if(dimension < 0)
  {
    for(i = 0; i < THTensor_(nDimension)(a); i++)
    {
      if(THTensor_(size)(a, i) == 3)
      {
        dimension = i;
        break;
      }
    }
    if(dimension < 0)
      THError("no dimension of size 3");
  }

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(a), 3, "dimension out of range");
  THArgCheck(THTensor_(size)(a, dimension) == 3, 3, "dimension size is not 3");

  THTensor_(resizeAs)(r_, a);

  TH_GEN_TENSOR_DIM_APPLY3(a, b, r_, dimension,
                           r__data[0*r__stride] = a_data[1*a_stride]*b_data[2*b_stride] - a_data[2*a_stride]*b_data[1*b_stride];
                           r__data[1*r__stride] = a_data[2*a_stride]*b_data[0*b_stride] - a_data[0*a_stride]*b_data[2*b_stride];
                           r__data[2*r__stride] = a_data[0*a_stride]*b_data[1*b_stride] - a_data[1*a_stride]*b_data[0*b_stride];);
}

void THLab_(zeros)(THLongStorage *size, THTensor *r_)
{
  THTensor_(resize)(r_, size);
  THTensor_(zero)(r_);
}

void THLab_(ones)(THLongStorage *size, THTensor *r_)
{
  THTensor_(resize)(r_, size);
  THTensor_(fill)(r_, 1);
}

void THLab_(diag)(THTensor *t, int k, THTensor *r_)
{
  THArgCheck(THTensor_(nDimension)(t) == 1 || THTensor_(nDimension)(t) == 2, 1, "matrix or a vector expected");

  if(THTensor_(nDimension)(t) == 1)
  {
    real *t_data = THTensor_(data)(t);
    long t_size = THTensor_(size)(t, 0);
    long sz = t_size + (k >= 0 ? k : -k);
    real *r__data;
    long r__stride;
    long i;

    THTensor_(resize2d)(r_, sz, sz);    
    THTensor_(zero)(r_);
    r__data = THTensor_(data)(r_);
    r__stride = THTensor_(stride)(r_, 1);
    r__data += (k >= 0 ? k : k*r__stride);

    for(i = 0; i < t_size; i++)
      r__data[i*(r__stride+1)] = t_data[i];
  }
  else
  {
    real *t_data = THTensor_(data)(t);
    long t_stride = THTensor_(stride)(t, 1);
    long sz;
    real *r__data;
    long i;

    if(k >= 0)
      sz = THMin(THTensor_(size)(t, 0), THTensor_(size)(t, 1)-k);
    else
      sz = THMin(THTensor_(size)(t, 0)+k, THTensor_(size)(t, 1));
    THTensor_(resize1d)(r_, sz);
    r__data = THTensor_(data)(r_);

    t_data += (k >= 0 ? k : k*t_stride);
    for(i = 0; i < sz; i++)
      r__data[i] = t_data[i*(t_stride+1)];
  }
}

void THLab_(eye)(long n, long m, THTensor *r_)
{
  real *r__data;
  long i, sz;

  THArgCheck(n > 0, 1, "invalid argument");
  THArgCheck(m > 0, 1, "invalid argument");

  THTensor_(resize2d)(r_, n, m);
  THTensor_(zero)(r_);

  i = 0;
  r__data = THTensor_(data)(r_);
  sz = THMin(THTensor_(size)(r_, 0), THTensor_(size)(r_, 1));
  for(i = 0; i < sz; i++)
    r__data[i*(1+m)] = 1;
}


void THLab_(range)(real xmin, real xmax, real step, THTensor *r_)
{
  long size;

  THArgCheck(step > 0, 3, "step must be a positive number");
  THArgCheck(xmax > xmin, 2, "upper bound must be larger than lower bound");

  size = (long)((xmax-xmin)/step+1);
  
  THTensor_(resize1d)(r_, size);

  TH_TENSOR_APPLY(r_, r__data[i] = xmin + ((real)i)*step;);
}

void THLab_(randperm)(long n, THTensor *r_)
{
  real *r__data;
  long i;

  THArgCheck(n > 0, 1, "must be strictly positive");

  THTensor_(resize1d)(r_, n);
  r__data = THTensor_(data)(r_);

  for(i = 0; i < n; i++)
      r__data[i] = (real)(i+1);

  for(i = 0; i < n-1; i++)
  {
    long z = THRandom_random() % (n-i);
    real sav = r__data[i];
    r__data[i] = r__data[z+i];
    r__data[z+i] = sav;
  }
}

void THLab_(reshape)(THTensor *t, THLongStorage *size, THTensor *r_)
{
  THTensor_(resize)(r_, size);
  THTensor_(copy)(r_, t);
}

/* I cut and pasted (slightly adapted) the quicksort code from
   http://www.alienryderflex.com/quicksort/
   This public-domain C implementation by Darel Rex Finley.
   Thanks man :)
*/
#define  MAX_LEVELS  300
static void THLab_(quicksortascend)(real *arr, long *idx, long elements, long stride)
{
  long beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R, swap, pid;
  real piv;
  
  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L*stride];
      pid=idx[L*stride];
      while (L<R) {
        while (arr[R*stride]>=piv && L<R) R--; if (L<R) {idx[L*stride]=idx[R*stride]; arr[L*stride]=arr[R*stride]; L++;}
        while (arr[L*stride]<=piv && L<R) L++; if (L<R) {idx[R*stride]=idx[L*stride]; arr[R*stride]=arr[L*stride]; R--;} }
      idx[L*stride]=pid; arr[L*stride]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L;
      if (end[i]-beg[i]>end[i-1]-beg[i-1]) {
        swap=beg[i]; beg[i]=beg[i-1]; beg[i-1]=swap;
        swap=end[i]; end[i]=end[i-1]; end[i-1]=swap; }}
    else {
      i--; }}}

static void THLab_(quicksortdescend)(real *arr, long *idx, long elements, long stride)
{
  long beg[MAX_LEVELS], end[MAX_LEVELS], i=0, L, R, swap, pid;
  real piv;
  
  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L*stride];
      pid=idx[L*stride];
      while (L<R) {
        while (arr[R*stride]<=piv && L<R) R--; if (L<R) {idx[L*stride]=idx[R*stride]; arr[L*stride]=arr[R*stride]; L++;}
        while (arr[L*stride]>=piv && L<R) L++; if (L<R) {idx[R*stride]=idx[L*stride]; arr[R*stride]=arr[L*stride]; R--;} }
      idx[L*stride]=pid; arr[L*stride]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L;
      if (end[i]-beg[i]>end[i-1]-beg[i-1]) {
        swap=beg[i]; beg[i]=beg[i-1]; beg[i-1]=swap;
        swap=end[i]; end[i]=end[i-1]; end[i-1]=swap; }}
    else {
      i--; }}}

void THLab_(sort)(THTensor *t, int dimension, int descendingOrder, THTensor *rt_, THLongTensor *ri_)
{
  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "invalid dimension");

  THTensor_(resizeAs)(rt_, t);
  THTensor_(copy)(rt_, t);

  {
    THLongStorage *size = THTensor_(newSizeOf)(t);
    THLongTensor_resize(ri_, size);
    THLongStorage_free(size);
  }

  if(descendingOrder)
  {
    TH_TENSOR_DIM_APPLY2(real, Real, rt_, long, Long, ri_, dimension, 
                         long i;
                         for(i = 0; i < ri__size; i++)
                           ri__data[i*ri__stride] = i;
                         THLab_(quicksortdescend)(rt__data, ri__data, rt__size, rt__stride);)
  }
  else
  {
    TH_TENSOR_DIM_APPLY2(real, Real, rt_, long, Long, ri_, dimension,
                         long i;
                         for(i = 0; i < ri__size; i++)
                           ri__data[i*ri__stride] = i;
                         THLab_(quicksortascend)(rt__data, ri__data, rt__size, rt__stride);)
  }
}

void THLab_(tril)(THTensor *t, long k, THTensor *r_)
{
  long stride, t_size_r, t_size_c;
  real *t_data, *r__data;
  long r, c;

  THArgCheck(THTensor_(nDimension)(t) == 2, 1, "not a matrix");

  THTensor_(resizeAs)(r_, t);

  stride = THTensor_(stride)(t, 1);
  t_size_r = THTensor_(size)(t, 0);
  t_size_c = THTensor_(size)(t, 1);
  r__data = THTensor_(data)(r_);
  t_data = THTensor_(data)(t);

  for(r = 0; r < t_size_r; r++)
  {
    long sz = THMin(r+k, t_size_c);
    for(c = THMax(0, r+k); c < t_size_c; c++)
      r__data[r*stride+c] = 0;
    for(c = 0; c < sz; c++)
      r__data[r*stride+c] = t_data[r*stride+c];
  }
}

void THLab_(triu)(THTensor *t, long k, THTensor *r_)
{
  long stride, t_size_r, t_size_c;
  real *t_data, *r__data;
  long r, c;

  THArgCheck(THTensor_(nDimension)(t) == 2, 1, "not a matrix");

  THTensor_(resizeAs)(r_, t);

  stride = THTensor_(stride)(t, 1);
  t_size_r = THTensor_(size)(t, 0);
  t_size_c = THTensor_(size)(t, 1);
  r__data = THTensor_(data)(r_);
  t_data = THTensor_(data)(t);

  for(r = 0; r < t_size_r; r++)
  {
    long sz = THMin(r+k, t_size_c);
    for(c = THMax(0, r+k); c < t_size_c; c++)
      r__data[r*stride+c] = t_data[r*stride+c];
    for(c = 0; c < sz; c++)
      r__data[r*stride+c] = 0;
  }
}

/* floating point only now */

#if defined(TH_REAL_IS_FLOAT) || defined(TH_REAL_IS_DOUBLE)

#define LAB_IMPLEMENT_BASIC_FUNCTION(NAME, CFUNC)             \
  void THLab_(NAME)(THTensor *t, THTensor *r_)                \
  {                                                           \
    THTensor_(resizeAs)(r_, t);                               \
    TH_TENSOR_APPLY2(t, r_, r__data[i] = CFUNC(t_data[i]););  \
  }                                                           \

#define LAB_IMPLEMENT_BASIC_FUNCTION_VALUE(NAME, CFUNC)              \
  void THLab_(NAME)(THTensor *t, real value, THTensor *r_)           \
  {                                                                  \
    THTensor_(resizeAs)(r_, t);                                      \
    TH_TENSOR_APPLY2(t, r_, r__data[i] = CFUNC(t_data[i], value););  \
  }                                                                  \

LAB_IMPLEMENT_BASIC_FUNCTION(log,log)
LAB_IMPLEMENT_BASIC_FUNCTION(log1p,log1p)
LAB_IMPLEMENT_BASIC_FUNCTION(exp,exp)
LAB_IMPLEMENT_BASIC_FUNCTION(cos,cos)
LAB_IMPLEMENT_BASIC_FUNCTION(acos,acos)
LAB_IMPLEMENT_BASIC_FUNCTION(cosh,cosh)
LAB_IMPLEMENT_BASIC_FUNCTION(sin,sin)
LAB_IMPLEMENT_BASIC_FUNCTION(asin,asin)
LAB_IMPLEMENT_BASIC_FUNCTION(sinh,sinh)
LAB_IMPLEMENT_BASIC_FUNCTION(tan,tan)
LAB_IMPLEMENT_BASIC_FUNCTION(atan,atan)
LAB_IMPLEMENT_BASIC_FUNCTION(tanh,tanh)
LAB_IMPLEMENT_BASIC_FUNCTION_VALUE(pow,pow)
LAB_IMPLEMENT_BASIC_FUNCTION(sqrt,sqrt)
LAB_IMPLEMENT_BASIC_FUNCTION(ceil,ceil)
LAB_IMPLEMENT_BASIC_FUNCTION(floor,floor)
LAB_IMPLEMENT_BASIC_FUNCTION(abs,fabs)

void THLab_(mean)(THTensor *t, int dimension, THTensor *r_)
{
  THLongStorage *dim;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 2, "invalid dimension");

  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(r_, dim);
  THLongStorage_free(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                             sum += t_data[i*t_stride];
                           *r__data = sum/t_size;);
}

void THLab_(std)(THTensor *t, int dimension, int flag, THTensor *r_)
{
  THLongStorage *dim;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 3, "invalid dimension");

  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(r_, dim);
  THLongStorage_free(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           real sum2 = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             real z = t_data[i*t_stride];
                             sum += z;
                             sum2 += z*z;
                           }

                           if(flag)
                           {
                             sum /= t_size;
                             sum2 /= t_size;
                             sum2 -= sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sqrt(sum2);
                           }
                           else
                           {
                             sum /= t_size;
                             sum2 /= t_size-1;
                             sum2 -= ((real)t_size)/((real)(t_size-1))*sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sqrt(sum2);
                           });
}

void THLab_(var)(THTensor *t, int dimension, int flag, THTensor *r_)
{
  THLongStorage *dim;

  THArgCheck(dimension >= 0 && dimension < THTensor_(nDimension)(t), 3, "invalid dimension");

  dim = THTensor_(newSizeOf)(t);
  THLongStorage_set(dim, dimension, 1);
  THTensor_(resize)(r_, dim);
  THLongStorage_free(dim);

  TH_GEN_TENSOR_DIM_APPLY2(t, r_, dimension,
                           real sum = 0;
                           real sum2 = 0;
                           long i;
                           for(i = 0; i < t_size; i++)
                           {
                             real z = t_data[i*t_stride];
                             sum += z;
                             sum2 += z*z;
                           }

                           if(flag)
                           {
                             sum /= t_size;
                             sum2 /= t_size;
                             sum2 -= sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sum2;
                           }
                           else
                           {
                             sum /= t_size;
                             sum2 /= t_size-1;
                             sum2 -= ((real)t_size)/((real)(t_size-1))*sum*sum;
                             sum2 = (sum2 < 0 ? 0 : sum2);
                             *r__data = sum2;
                           });
}

void THLab_(norm)(THTensor *t, real value, real *norm_)
{
  *norm_ = THTensor_(norm)(t, value);
}

void THLab_(dist)(THTensor *a, THTensor *b, real value, real *dist_)
{ 
  *dist_ = THTensor_(dist)(a, b, value);
}

void THLab_(linspace)(real a, real b, long n, THTensor *r_)
{
  THArgCheck(n > 0, 3, "invalid number of points");
  THArgCheck(a <= b, 2, "end range should be greater than start range");
  
  THTensor_(resize1d)(r_, n);

  TH_TENSOR_APPLY(r_, r__data[i] = a + ((real)i)*(b-a)/((real)(n-1)););
}

void THLab_(logspace)(real a, real b, long n, THTensor *r_)
{
  THArgCheck(n > 0, 3, "invalid number of points");
  THArgCheck(a <= b, 2, "end range should be greater than start range");
  
  THTensor_(resize1d)(r_, n);

  TH_TENSOR_APPLY(r_, r__data[i] = pow(10.0, a + ((real)i)*(b-a)/((real)(n-1))););
}

void THLab_(rand)(THLongStorage *size, THTensor *r_)
{
  THTensor_(resize)(r_, size);
  THTensor_(uniform)(r_, 0, 1);
}

void THLab_(randn)(THLongStorage *size, THTensor *r_)
{
  THTensor_(resize)(r_, size);
  THTensor_(normal)(r_, 0, 1);
}

#endif /* floating point only part */
#endif
