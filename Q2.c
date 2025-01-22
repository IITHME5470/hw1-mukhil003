#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void read_matrix(int n,double **matrix,const char *filename)
{
	int i,j;
	
	FILE *file = fopen(filename,"r");
	if(file == NULL)
	{
		printf("Unable to open file '%s'.\n",filename);
		exit(1);
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			if(fscanf(file,"%lf,",&matrix[i][j]) != 1)
			{
				printf("ERROR!");
				exit(1);
				fclose(file);
			}
		}
	}
	fclose(file);
}

void read_vector(int n,double *vector,const char *filename)
{
	FILE *file = fopen(filename,"r");
	if(file == NULL)
	{
		printf("Unable to open file '%s'. \n",filename);
		exit(1);
	}
	
	int i;
	for(i=0;i<n;i++)
	{
		if(fscanf(file,"%lf,",&vector[i]) != 1)
		{
			printf("ERROR!");
			exit(1);
			fclose(file);
		}
	}
	fclose(file);
				
}

int is_eigenvector(int n,double **matrix,double *vector,double *eigenvalue)
{
	int i,j;
	double *AX = (double *)malloc(n*sizeof(double));
	for(i=0;i<n;i++)
	{
		AX[i] = 0.0;
		for(j=0;j<n;j++)
			AX[i] += matrix[i][j] * vector[j];
	}

	if(vector[0] != 0)
	{
		
		*eigenvalue = AX[0]/vector[0];    //Assume vector[0] != 0
		for(i=0;i<n;i++)
		{
			if(vector[i] != 0)
			{
				if(fabs(AX[i]/vector[i] - *eigenvalue) > 1e-12)
				{
					free(AX);
					return 0;
				}
			}

		}
	
		free(AX);
		return 1;
	}
	else if(AX[0] != 0)
	{
		free(AX);
		return 0;
	}
	else
	{
		*eigenvalue = 0.0;
		int m = 0;
		for(i=0;i<n;i++)
		{
			if(AX[i] != 0 && vector[i] != 0)
			{
				*eigenvalue = AX[i]/vector[i];
				break;
			}
		}
		for(i=0;i<n;i++)
		{
			if(AX[i] != 0 && vector[i] != 0)
			{
				if(fabs(AX[i]/vector[i] - *eigenvalue) > 1e-12)
				{
					free(AX);
					return 0;
				}
				
			}
			if(vector[i] != 0)
				m++;
		}
		if(m == 0)
		{
			free(AX);
			return 0;
		}
		free(AX);
		return 1;
	}
}

void append_eigenvalue(const char *filename,double eigenvalue,int is_eigen)
{
	FILE *file = fopen(filename,"a");
	if(file == NULL)
	{
		printf("Unable to open the file '%s' for appending. \n",filename);
		exit(1);
	}
	if(is_eigen)
	{
		fprintf(file,",%.15e",eigenvalue);
	}
	fclose(file);
}


int main()
{
	int n,i,k,vecnum;

	FILE *fptr = fopen("input.in","r");
	if(fptr == NULL)
	{
		printf("ERROR! Unable to open the file.\n");
		return(1);
	}

	if(fscanf(fptr,"%d",&n) != 1)
	{
		printf("ERROR!\n");
		fclose(fptr);
		return(1);
	}
	fclose(fptr);

	double **matrix = (double **)malloc(n*sizeof(double*));
	if(matrix == NULL)
	{
		printf("Memory allocation failed!\n");
		return(1);
	}

	for(i=0;i<n;i++)
	{
		matrix[i] = (double *)malloc(n*sizeof(double));
		if(matrix[i] == NULL)
		{
			printf("Memory allocation failed!\n");
			for(k=0;k<i;k++)
				free(matrix[k]);
			free(matrix);
			return(1);
		}
	}
	double *vector = (double *)malloc(n*sizeof(double));
	if(vector == NULL)
	{
		printf("Memory allocation failed!\n");
		return(1);
	}

	char matrix_filename[50];
	snprintf(matrix_filename,sizeof(matrix_filename),"inputfiles/mat_%06d.in",n);
	read_matrix(n,matrix,matrix_filename);


	for(vecnum = 1;;vecnum++)
	{
		char vector_filename[50];
		snprintf(vector_filename,sizeof(vector_filename),"inputfiles/vec_%06d_%06d.in",n,vecnum);

		FILE *vec_file = fopen(vector_filename,"r");
		if(vec_file == NULL)
			break;
		fclose(vec_file);

		read_vector(n,vector,vector_filename);

		double eigenvalue;
		int is_eigen = is_eigenvector(n,matrix,vector,&eigenvalue);

		append_eigenvalue(vector_filename,eigenvalue,is_eigen);

		if(is_eigen)
			printf("%s : Yes : %.15e\n",vector_filename,eigenvalue);
		else
			printf("%s : Not an eigenvector\n",vector_filename);
		

	}

	for(i=0;i<n;i++)
		free(matrix[i]);
	free(matrix);
	free(vector);
	
	return(0);
	
	

}
