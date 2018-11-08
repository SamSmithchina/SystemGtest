USE [kdfixgw_GTJA]
GO
/****** Object:  Table [dbo].[system_status]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[system_status](
	[keys] [varchar](128) NOT NULL,
	[value] [varchar](128) NOT NULL,
	[reserve] [varchar](64) NULL,
PRIMARY KEY CLUSTERED 
(
	[keys] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[system_config]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[system_config](
	[keyname] [varchar](64) NOT NULL,
	[value] [varchar](64) NULL,
PRIMARY KEY CLUSTERED 
(
	[keyname] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[order_info]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[order_info](
	[status] [varchar](8) NULL,
	[sessionid] [varchar](128) NOT NULL,
	[clordid] [varchar](128) NOT NULL,
	[clordid_cancl] [varchar](128) NULL,
	[orderid] [varchar](64) NOT NULL,
	[ordersno] [varchar](64) NOT NULL,
	[market] [varchar](1) NOT NULL,
	[secuid] [varchar](10) NOT NULL,
	[fundid] [varchar](12) NOT NULL,
	[stkcode] [varchar](10) NOT NULL,
	[bsflag] [varchar](32) NOT NULL,
	[price] [decimal](12, 4) NOT NULL,
	[qty] [int] NULL,
	[qty_cancel] [int] NULL,
	[qty_match] [int] NULL,
	[amt_match] [money] NULL,
	[price_avg] [decimal](12, 4) NULL,
	[last_shares] [int] NULL,
	[last_px] [decimal](12, 4) NULL,
	[listid] [varchar](128) NULL,
	[liststatus] [varchar](32) NULL,
	[trans_type] [varchar](32) NULL,
	[order_date] [int] NOT NULL,
	[delivertocompid] [varchar](128) NOT NULL,
 CONSTRAINT [PK__order_in__836EA03307020F21] PRIMARY KEY CLUSTERED 
(
	[order_date] ASC,
	[sessionid] ASC,
	[clordid] ASC,
	[delivertocompid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[message_info]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[message_info](
	[keys] [varchar](128) NOT NULL,
	[value] [varchar](128) NOT NULL,
	[reserve] [varchar](64) NULL,
	[reserve2] [varchar](64) NULL,
	[reserve3] [varchar](32) NULL,
PRIMARY KEY CLUSTERED 
(
	[keys] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[logoner_info]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[logoner_info](
	[info] [varchar](128) NOT NULL,
	[sessionid] [varchar](128) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[sessionid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ids]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ids](
	[sessionid] [varchar](128) NOT NULL,
	[type] [char](1) NOT NULL,
	[id] [varchar](64) NOT NULL,
	[order_date] [int] NOT NULL,
	[delivertocompid] [varchar](128) NOT NULL,
 CONSTRAINT [PK__ids__8629FFFE108B795B] PRIMARY KEY CLUSTERED 
(
	[sessionid] ASC,
	[type] ASC,
	[id] ASC,
	[order_date] ASC,
	[delivertocompid] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[customer]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[customer](
	[beginstring] [char](8) NOT NULL,
	[sendercompid] [varchar](64) NOT NULL,
	[targetcompid] [varchar](64) NOT NULL,
	[session_qualifier] [varchar](64) NOT NULL,
	[creation_time] [datetime] NOT NULL,
	[incoming_seqnum] [int] NOT NULL,
	[outgoing_seqnum] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[beginstring] ASC,
	[sendercompid] ASC,
	[targetcompid] ASC,
	[session_qualifier] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[cjhb]    Script Date: 11/05/2018 11:01:40 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[cjhb](
	[cjhm] [varchar](32) NOT NULL,
	[cjhb_date] [int] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[cjhm] ASC,
	[cjhb_date] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Default [DF__ids__delivertoco__1273C1CD]    Script Date: 11/05/2018 11:01:40 ******/
ALTER TABLE [dbo].[ids] ADD  DEFAULT ('NoDeliverToCompid') FOR [delivertocompid]
GO
/****** Object:  Default [DF__order_inf__deliv__08EA5793]    Script Date: 11/05/2018 11:01:40 ******/
ALTER TABLE [dbo].[order_info] ADD  DEFAULT ('NoDeliverToCompid') FOR [delivertocompid]
GO
