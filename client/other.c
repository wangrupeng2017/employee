
#include "other.h"

// 管理员查询相关方法
int adminQueryBusiness(int file_descriptor, LoginResultModel * login_model)
{
	int choose = 0;

	printf("********************  请选择查询的方式  ***********************\n");
	printf("**************  1:员工号 2:用户名 0:退出 **********************\n");
	printf("***************************************************************\n");

Input_Label:
	fprintf(stderr, "请选择查询的方式：");
	scanf("%d", &choose);
	while(getchar() != '\n');
	if (choose<0 || choose>2)
	{
		printf("输入有误，请重新输入\n");
		goto Input_Label;
	}


	int ret = 0;
	RequestInfo         req_head = {0};
	EmployeeQueryModel  req_data = {0};
	ResponseInfo        res_head = {0};
	EmployeeQueryResult res_data[20] = {0};
	switch (choose)
	{
		case 1:  //员工号
			printf("请输入查询的员工号：");
			scanf("%d", &req_data.empno);		
			while(getchar() != '\n');
			break;
		case 2:  //用户名
			printf("请输入查询的员工名：");
			scanf("%s", req_data.name);		
			while(getchar() != '\n');
			break;
		case 0:  //退出
			return FuncNormal;
			break;
	}
	req_head.type = EmployeeQuery;
	req_head.size = sizeof(req_data);

	// 发送查询请求
	ret = request(file_descriptor, 
			&req_head, sizeof(req_head), &req_data, sizeof(req_data),
			&res_head, sizeof(res_head), res_data, sizeof(res_data));

	// 查询结果员工数量
	int employee_count = res_head.size/sizeof(EmployeeQueryResult);
	if (employee_count == 0)
	{
		printf("未查询到指定条件的员工信息, 请换个条件试一试\n");
		return FuncNormal;
	}

	// 打印员工信息
	int i = 0;
	for (i=0; i<employee_count; i++)
	{
		EmployeeQueryResult e = res_data[i];
		printf("[%d]\t%d\t%s\t%s\t%s\t%d\t%d\t%s\n", 
				i+1, e.empno, e.name, e.pwd, e.sex==Male?"男":"女",
				e.age, e.salary, e.department);
	}

	return FuncNormal;
}

