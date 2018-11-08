USE [Ashare_OIW]
GO
/****** Object:  Table [dbo].[test_tab]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[test_tab](
	[f1] [int] NULL,
	[f2] [varchar](30) NULL
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[snoordwth]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[snoordwth](
	[lastdate] [int] NULL,
	[lastsno] [int] NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[reqresp]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[reqresp](
	[reqnum] [int] NOT NULL,
	[reqid] [char](3) NOT NULL,
	[reff] [char](10) NOT NULL,
	[pbu] [char](5) NOT NULL,
	[securityid] [char](6) NOT NULL,
	[recordtimestamp] [char](21) NULL,
	[ordstatus] [char](1) NOT NULL,
	[remark] [char](5) NULL,
	[reqtext] [char](1024) NULL,
	[resptext] [char](1024) NULL,
	[localrespnum] [int] NULL,
	[retpbu] [char](5) NULL,
 CONSTRAINT [PK_REQRESP] PRIMARY KEY CLUSTERED 
(
	[reqnum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[pubdata]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[pubdata](
	[pubnum] [int] NOT NULL,
	[bcasttype] [char](2) NOT NULL,
	[setid] [char](3) NOT NULL,
	[seqnum] [int] NOT NULL,
	[recordtimestamp] [char](21) NULL,
	[mdtext] [char](1024) NULL,
 CONSTRAINT [PK_PUBDATA] PRIMARY KEY CLUSTERED 
(
	[pubnum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY],
 CONSTRAINT [UNIQUE_PUBDATA] UNIQUE NONCLUSTERED 
(
	[bcasttype] ASC,
	[setid] ASC,
	[seqnum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[execreport]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[execreport](
	[execnum] [int] NOT NULL,
	[bcasttype] [char](2) NOT NULL,
	[setid] [char](3) NOT NULL,
	[seqnum] [int] NOT NULL,
	[recordtimestamp] [char](21) NULL,
	[execreporttext] [char](1024) NULL,
 CONSTRAINT [PK_EXECREPORT] PRIMARY KEY CLUSTERED 
(
	[execnum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY],
 CONSTRAINT [UNIQUE_EXECREPORT] UNIQUE NONCLUSTERED 
(
	[bcasttype] ASC,
	[setid] ASC,
	[seqnum] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ashare_ordwth2]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ashare_ordwth2](
	[rec_num] [int] NOT NULL,
	[date] [char](8) NULL,
	[time] [char](8) NULL,
	[reff] [char](10) NULL,
	[acc] [char](10) NULL,
	[stock] [char](6) NULL,
	[bs] [char](1) NULL,
	[price] [char](8) NULL,
	[qty] [char](8) NULL,
	[status] [char](1) NULL,
	[qty2] [char](8) NULL,
	[remark] [char](30) NULL,
	[status1] [char](1) NULL,
	[teordernum] [char](8) NULL,
	[owflag] [char](3) NULL,
	[ordrec] [char](8) NULL,
	[firmid] [char](5) NULL,
	[branchid] [char](5) NULL,
	[checkord] [binary](16) NULL,
	[rec_num2] [int] IDENTITY(1,1) NOT NULL,
 CONSTRAINT [PK_ASHARE_ORDWTH2] PRIMARY KEY CLUSTERED 
(
	[rec_num2] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ashare_ordwth]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ashare_ordwth](
	[rec_num] [int] NOT NULL,
	[date] [char](8) NULL,
	[time] [char](8) NULL,
	[reff] [char](10) NULL,
	[acc] [char](10) NULL,
	[stock] [char](6) NULL,
	[bs] [char](1) NULL,
	[price] [char](8) NULL,
	[qty] [char](8) NULL,
	[status] [char](1) NULL,
	[owflag] [char](3) NULL,
	[ordrec] [char](8) NULL,
	[firmid] [char](5) NULL,
	[branchid] [char](5) NULL,
	[checkord] [binary](16) NULL,
 CONSTRAINT [PK_ASHARE_ORDWTH] PRIMARY KEY CLUSTERED 
(
	[rec_num] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ashare_cjhb]    Script Date: 11/05/2018 11:02:53 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ashare_cjhb](
	[rec_num] [int] IDENTITY(1,1) NOT NULL,
	[gddm] [char](10) NULL,
	[gdxm] [char](8) NULL,
	[bcrq] [char](8) NULL,
	[cjbh] [int] NOT NULL,
	[gsdm] [char](5) NULL,
	[cjsl] [char](10) NULL,
	[bcye] [char](10) NULL,
	[zqdm] [char](6) NOT NULL,
	[sbsj] [char](6) NULL,
	[cjsj] [char](6) NULL,
	[cjjg] [char](8) NULL,
	[cjje] [char](12) NULL,
	[sqbh] [char](10) NULL,
	[bs] [char](1) NOT NULL,
	[mjbh] [char](5) NULL,
 CONSTRAINT [PK_ASHARE_CJHB] PRIMARY KEY CLUSTERED 
(
	[rec_num] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
