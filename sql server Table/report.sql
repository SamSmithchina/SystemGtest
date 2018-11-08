USE [report]
GO
/****** Object:  Table [dbo].[t_fundmonitor_A13]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[t_fundmonitor_A13](
	[serverid] [dbo].[dtsno] NOT NULL,
	[zqgsdm] [dbo].[dtchar8] NOT NULL,
	[jyrq] [dbo].[dtchar8] NOT NULL,
	[ysfyy] [dbo].[dtchar4] NOT NULL,
	[ysyflx] [dbo].[dtchar2] NOT NULL,
	[dfjg] [dbo].[dtchar4] NOT NULL,
	[scdm] [dbo].[dtchar2] NOT NULL,
	[ywlx] [dbo].[dtchar4] NOT NULL,
	[bz] [dbo].[dtchar4] NOT NULL,
	[ysyfje] [dbo].[dtamt] NOT NULL,
	[sjsfrq] [dbo].[dtchar8] NOT NULL,
	[ysfrq] [dbo].[dtchar8] NOT NULL,
	[zyxx] [dbo].[dtkinds] NOT NULL,
	[sjrq] [dbo].[dtchar8] NOT NULL,
	[frqsrq] [dbo].[dtchar8] NOT NULL,
	[frqsje] [dbo].[dtamt] NOT NULL,
	[qszc] [dbo].[dtamt] NOT NULL,
	[sjlx] [dbo].[dtkind] NOT NULL,
	[bsfs] [dbo].[dtkind] NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[sysmgr_objects]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[sysmgr_objects](
	[serverid] [dbo].[dtsno] NOT NULL,
	[objecttype] [dbo].[dtchar4] NOT NULL,
	[objectname] [dbo].[dtchar64] NOT NULL,
	[createdate] [dbo].[dtdate] NOT NULL,
	[lastdate] [dbo].[dtdate] NOT NULL,
 CONSTRAINT [pk_sysmgrobjects] PRIMARY KEY CLUSTERED 
(
	[objectname] ASC,
	[objecttype] ASC,
	[serverid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[repltable]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[repltable](
	[dbname] [varchar](32) NOT NULL,
	[pubname] [varchar](32) NOT NULL,
	[tablename] [varchar](64) NOT NULL,
	[serverid] [dbo].[dtsno] NOT NULL,
	[bzxx] [varchar](64) NOT NULL,
 CONSTRAINT [pk_repltable] PRIMARY KEY CLUSTERED 
(
	[dbname] ASC,
	[pubname] ASC,
	[tablename] ASC,
	[serverid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A19]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A19](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[zjzhlb] [char](2) NOT NULL,
	[scdm] [char](2) NOT NULL,
	[fzlb] [char](2) NOT NULL,
	[fzje] [varchar](20) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A18]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A18](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[zjzhlb] [char](2) NOT NULL,
	[zqzh] [varchar](32) NOT NULL,
	[scdm] [char](2) NOT NULL,
	[zqlb] [char](2) NOT NULL,
	[zqdm] [varchar](20) NOT NULL,
	[ltlx] [char](1) NOT NULL,
	[ccsl] [varchar](20) NOT NULL,
	[ccsz] [varchar](20) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A17]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A17](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[zjzhlb] [char](2) NOT NULL,
	[bz] [char](3) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[qsrq] [char](8) NOT NULL,
	[dzrq] [char](8) NOT NULL,
	[zqzh] [varchar](32) NOT NULL,
	[zqlsh] [varchar](20) NOT NULL,
	[ywdl] [char](3) NOT NULL,
	[ywlx] [char](3) NOT NULL,
	[zyxx] [varchar](120) NOT NULL,
	[fsje] [varchar](20) NOT NULL,
	[scdm] [char](2) NOT NULL,
	[sqbh] [varchar](24) NOT NULL,
	[cjbh] [varchar](18) NOT NULL,
	[cpdm] [varchar](20) NOT NULL,
	[cpmc] [varchar](120) NOT NULL,
	[cplb] [char](2) NOT NULL,
	[cjsl] [varchar](20) NOT NULL,
	[qsje] [varchar](20) NOT NULL,
	[sxf] [varchar](20) NOT NULL,
	[zdxx] [varchar](256) NOT NULL,
	[wtjrfs] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A14]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A14](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[bz] [char](3) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[zqlsh] [varchar](20) NOT NULL,
	[ywlx] [char](3) NOT NULL,
	[zyxx] [varchar](120) NOT NULL,
	[fsje] [varchar](20) NOT NULL,
	[zfljlx] [char](2) NOT NULL,
	[zfljjgmc] [varchar](120) NOT NULL,
	[dfzhzh] [varchar](32) NOT NULL,
	[dfzhmc] [varchar](150) NOT NULL,
	[dfzhkhh] [varchar](150) NOT NULL,
	[zfjggjzhzh] [varchar](32) NOT NULL,
	[jsrq] [char](8) NOT NULL,
	[zdxx] [varchar](256) NOT NULL,
	[wtjrfs] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL,
	[by1] [varchar](32) NOT NULL,
	[by2] [varchar](150) NOT NULL,
	[by3] [varchar](150) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A13_bak]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A13_bak](
	[serverid] [dbo].[dtsno] NOT NULL,
	[zqgsdm] [dbo].[dtchar8] NOT NULL,
	[jyrq] [dbo].[dtchar8] NOT NULL,
	[ysfyy] [dbo].[dtchar4] NOT NULL,
	[ysyflx] [dbo].[dtchar2] NOT NULL,
	[dfjg] [dbo].[dtchar4] NOT NULL,
	[scdm] [dbo].[dtchar2] NOT NULL,
	[ywlx] [dbo].[dtchar4] NOT NULL,
	[bz] [dbo].[dtchar4] NOT NULL,
	[ysyfje] [dbo].[dtamt] NOT NULL,
	[sjsfrq] [dbo].[dtchar8] NOT NULL,
	[ysfrq] [dbo].[dtchar8] NOT NULL,
	[zyxx] [dbo].[dtkinds] NOT NULL,
	[sjrq] [dbo].[dtchar8] NOT NULL,
	[frqsrq] [dbo].[dtchar8] NOT NULL,
	[frqsje] [dbo].[dtamt] NOT NULL,
	[qszc] [dbo].[dtamt] NOT NULL,
	[sjlx] [dbo].[dtkind] NOT NULL,
	[bsfs] [dbo].[dtkind] NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A13]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A13](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[ysfyy] [char](3) NOT NULL,
	[ysyflx] [char](2) NOT NULL,
	[dfjg] [char](3) NOT NULL,
	[scdm] [char](2) NOT NULL,
	[ywlx] [char](3) NOT NULL,
	[bz] [char](3) NOT NULL,
	[ysyfje] [numeric](17, 2) NOT NULL,
	[sjsfrq] [char](8) NOT NULL,
	[ysfrq] [char](8) NOT NULL,
	[zyxx] [varchar](120) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A12]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A12](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[zjzhlb] [char](1) NOT NULL,
	[sfqydm] [varchar](32) NOT NULL,
	[glzh] [varchar](32) NOT NULL,
	[jszh] [varchar](32) NOT NULL,
	[bz] [char](3) NOT NULL,
	[zjzhye] [varchar](20) NOT NULL,
	[zjkqye] [varchar](20) NOT NULL,
	[zjkyye] [varchar](20) NOT NULL,
	[djje] [varchar](20) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A11]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A11](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[sfqydm] [varchar](32) NOT NULL,
	[glzh] [varchar](32) NOT NULL,
	[jszh] [varchar](32) NOT NULL,
	[jysj] [char](6) NOT NULL,
	[zqlsh] [varchar](20) NOT NULL,
	[yhlsh] [varchar](20) NOT NULL,
	[fqf] [char](1) NOT NULL,
	[jylx] [char](3) NOT NULL,
	[bz] [char](3) NOT NULL,
	[fsje] [varchar](20) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A10]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A10](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[sfqydm] [varchar](32) NOT NULL,
	[glzh] [varchar](32) NOT NULL,
	[jszh] [varchar](32) NOT NULL,
	[jysj] [char](6) NOT NULL,
	[zqlsh] [varchar](20) NOT NULL,
	[yhlsh] [varchar](20) NOT NULL,
	[cqlx] [char](1) NOT NULL,
	[bz] [char](3) NOT NULL,
	[fsje] [char](20) NOT NULL,
	[zdxx] [varchar](256) NOT NULL,
	[wtjrfs] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A09]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A09](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[sfqydm] [varchar](32) NOT NULL,
	[glzh] [varchar](32) NOT NULL,
	[jszh] [varchar](32) NOT NULL,
	[zqlsh] [varchar](20) NOT NULL,
	[jxlx] [char](1) NOT NULL,
	[bz] [char](3) NOT NULL,
	[lx] [varchar](20) NOT NULL,
	[lxs] [varchar](20) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A08]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A08](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[bz] [char](3) NOT NULL,
	[jyrq] [char](8) NOT NULL,
	[qsrq] [char](8) NOT NULL,
	[dzrq] [char](8) NOT NULL,
	[zqlsh] [varchar](20) NOT NULL,
	[gllsh] [varchar](20) NOT NULL,
	[ywdl] [char](3) NOT NULL,
	[ywlx] [char](3) NOT NULL,
	[zyxx] [varchar](120) NOT NULL,
	[fsje] [varchar](20) NOT NULL,
	[scdm] [char](2) NOT NULL,
	[jydy] [char](6) NOT NULL,
	[sqbh] [varchar](24) NOT NULL,
	[cjbh] [varchar](18) NOT NULL,
	[jjqrlsh] [varchar](20) NOT NULL,
	[zqzh] [char](12) NOT NULL,
	[jjjyzh] [varchar](17) NOT NULL,
	[zqzhlb] [char](2) NOT NULL,
	[jjtadm] [char](3) NOT NULL,
	[zqdm] [char](6) NOT NULL,
	[zqlb] [char](2) NOT NULL,
	[cjsl] [char](20) NOT NULL,
	[mmbz] [char](1) NOT NULL,
	[qsje] [varchar](20) NOT NULL,
	[yhs] [varchar](20) NOT NULL,
	[jsf] [varchar](20) NOT NULL,
	[ghf] [varchar](20) NOT NULL,
	[zgf] [varchar](20) NOT NULL,
	[qtfy1] [varchar](20) NOT NULL,
	[qtfy2] [varchar](20) NOT NULL,
	[qtfy3] [varchar](20) NOT NULL,
	[sxf] [varchar](20) NOT NULL,
	[wtsj] [char](6) NOT NULL,
	[cjsj] [char](6) NOT NULL,
	[zdxx] [varchar](256) NOT NULL,
	[wtjrfs] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A07_Z]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A07_Z](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[sfqydm] [varchar](32) NOT NULL,
	[glzh] [varchar](32) NOT NULL,
	[jszh] [varchar](32) NOT NULL,
	[jszhmc] [varchar](120) NOT NULL,
	[jszhkhh] [varchar](150) NOT NULL,
	[bz] [char](3) NOT NULL,
	[cgzt] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A07_Q]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A07_Q](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[sfqydm] [varchar](32) NOT NULL,
	[glzh] [varchar](32) NOT NULL,
	[jszh] [varchar](32) NOT NULL,
	[jszhmc] [varchar](120) NOT NULL,
	[jszhkhh] [varchar](150) NOT NULL,
	[bz] [char](3) NOT NULL,
	[cgzt] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A06_Z]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A06_Z](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[zqzh] [varchar](32) NOT NULL,
	[jjjyzh] [varchar](17) NOT NULL,
	[zqzhlb] [char](2) NOT NULL,
	[jjtadm] [char](3) NOT NULL,
	[zqzhzt] [char](2) NOT NULL,
	[qxbz] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A06_Q]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A06_Q](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[zqzh] [varchar](32) NOT NULL,
	[jjjyzh] [varchar](17) NOT NULL,
	[zqzhlb] [char](2) NOT NULL,
	[jjtadm] [char](3) NOT NULL,
	[zqzhzt] [char](2) NOT NULL,
	[qxbz] [char](1) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A05_Z]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A05_Z](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[khdm] [varchar](20) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[bz] [char](3) NOT NULL,
	[zjzhlb] [char](1) NOT NULL,
	[zjzhzt] [char](2) NOT NULL,
	[zhcglx] [char](1) NOT NULL,
	[zjzhgflx] [char](2) NOT NULL,
	[zjzhywlb] [char](1) NOT NULL,
	[khrq] [char](8) NOT NULL,
	[xhrq] [char](8) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A05_Q]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A05_Q](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[khdm] [varchar](20) NOT NULL,
	[zjzh] [varchar](20) NOT NULL,
	[bz] [char](3) NOT NULL,
	[zjzhlb] [char](1) NOT NULL,
	[zjzhzt] [char](2) NOT NULL,
	[zhcglx] [char](1) NOT NULL,
	[zjzhgflx] [char](2) NOT NULL,
	[zjzhywlb] [char](1) NOT NULL,
	[khrq] [char](8) NOT NULL,
	[xhrq] [char](8) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A04_Z]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A04_Z](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[khdm] [varchar](20) NOT NULL,
	[khkhrq] [char](8) NOT NULL,
	[khzxrq] [char](8) NOT NULL,
	[khmc] [varchar](120) NOT NULL,
	[khlx] [char](4) NOT NULL,
	[khzt] [char](2) NOT NULL,
	[zjlx1] [char](2) NOT NULL,
	[zjhm1] [varchar](60) NOT NULL,
	[zjyxjzr1] [char](8) NOT NULL,
	[zjlx2] [char](2) NOT NULL,
	[zjhm2] [varchar](60) NOT NULL,
	[zjyxjzr2] [char](8) NOT NULL,
	[khgj] [char](3) NOT NULL,
	[khxl] [char](2) NOT NULL,
	[khzy] [char](2) NOT NULL,
	[frdb] [varchar](120) NOT NULL,
	[frdbzjhm] [varchar](60) NOT NULL,
	[jbr] [varchar](120) NOT NULL,
	[jbrzjhm] [varchar](60) NOT NULL,
	[zcdz] [varchar](120) NOT NULL,
	[lxdz] [varchar](120) NOT NULL,
	[yzbm] [varchar](20) NOT NULL,
	[lxdh] [varchar](30) NOT NULL,
	[sjhm] [varchar](30) NOT NULL,
	[dzyx] [varchar](96) NOT NULL,
	[cgms] [char](6) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A04_Q]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A04_Q](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[khdm] [varchar](20) NOT NULL,
	[khkhrq] [char](8) NOT NULL,
	[khzxrq] [char](8) NOT NULL,
	[khmc] [varchar](120) NOT NULL,
	[khlx] [char](4) NOT NULL,
	[khzt] [char](2) NOT NULL,
	[zjlx1] [char](2) NOT NULL,
	[zjhm1] [varchar](60) NOT NULL,
	[zjyxjzr1] [char](8) NOT NULL,
	[zjlx2] [char](2) NOT NULL,
	[zjhm2] [varchar](60) NOT NULL,
	[zjyxjzr2] [char](8) NOT NULL,
	[khgj] [char](3) NOT NULL,
	[khxl] [char](2) NOT NULL,
	[khzy] [char](2) NOT NULL,
	[frdb] [varchar](120) NOT NULL,
	[frdbzjhm] [varchar](60) NOT NULL,
	[jbr] [varchar](120) NOT NULL,
	[jbrzjhm] [varchar](60) NOT NULL,
	[zcdz] [varchar](120) NOT NULL,
	[lxdz] [varchar](120) NOT NULL,
	[yzbm] [varchar](20) NOT NULL,
	[lxdh] [varchar](30) NOT NULL,
	[sjhm] [varchar](30) NOT NULL,
	[dzyx] [varchar](96) NOT NULL,
	[cgms] [char](6) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A03]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A03](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yhdm] [char](7) NOT NULL,
	[yhmc] [varchar](120) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A02]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A02](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[yybdm] [char](8) NOT NULL,
	[yybmc] [varchar](180) NOT NULL,
	[yybnbdm] [char](6) NOT NULL,
	[yybssjgxq] [char](2) NOT NULL,
	[yyblx] [char](2) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[fundmonitor_A01]    Script Date: 11/05/2018 11:04:37 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING OFF
GO
CREATE TABLE [dbo].[fundmonitor_A01](
	[serverid] [dbo].[dtsno] NOT NULL,
	[subsystem] [char](8) NOT NULL,
	[datadate] [dbo].[dtdate] NOT NULL,
	[zqgsdm] [char](8) NOT NULL,
	[zqgsmc] [varchar](120) NOT NULL,
	[jscyrdm] [char](6) NOT NULL,
	[jjxsrdm] [char](3) NOT NULL,
	[jscyrqsbh] [char](5) NOT NULL,
	[jscyrjszh] [char](6) NOT NULL,
	[frssjgxq] [char](2) NOT NULL,
	[sjrq] [char](8) NOT NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Default [DF__repltable__dbnam__09DE7BCC]    Script Date: 11/05/2018 11:04:37 ******/
ALTER TABLE [dbo].[repltable] ADD  DEFAULT ('run') FOR [dbname]
GO
/****** Object:  Default [DF__repltable__pubna__0AD2A005]    Script Date: 11/05/2018 11:04:37 ******/
ALTER TABLE [dbo].[repltable] ADD  DEFAULT ('run') FOR [pubname]
GO
/****** Object:  Default [DF__repltable__table__0BC6C43E]    Script Date: 11/05/2018 11:04:37 ******/
ALTER TABLE [dbo].[repltable] ADD  DEFAULT ('') FOR [tablename]
GO
/****** Object:  Default [DF__repltable__bzxx__0CBAE877]    Script Date: 11/05/2018 11:04:37 ******/
ALTER TABLE [dbo].[repltable] ADD  DEFAULT ('') FOR [bzxx]
GO
