#ifndef __SQLPREFIX_H__
#define __SQLPREFIX_H__
#include <iostream>

//SQL insert 命令前缀；
const static std::string strInsertSql_prefix = "INSERT INTO [Ashare_OIW].[dbo].[ashare_ordwth]([date],[time],[reff],[acc]"
", [stock], [bs], [price], [qty], [status]"
" , [owflag], [ordrec], [firmid], [branchid], [checkord],[rec_num])VALUES";

//清空ordwth
const std::string strDeleteOrdwth = "TRUNCATE TABLE [Ashare_OIW].[dbo].[ashare_ordwth];";

//清空ordwth2
const std::string strDeleteOrdwth2 = "TRUNCATE TABLE [Ashare_OIW].[dbo].[ashare_ordwth2];";

//清空cjhb
const std::string strDeleteCjhb = "TRUNCATE TABLE [Ashare_OIW].[dbo].[ashare_cjhb];";


#endif