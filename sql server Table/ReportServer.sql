USE [ReportServer]
GO
/****** Object:  Table [dbo].[Policies]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Policies](
	[PolicyID] [uniqueidentifier] NOT NULL,
	[PolicyFlag] [tinyint] NULL,
 CONSTRAINT [PK_Policies] PRIMARY KEY CLUSTERED 
(
	[PolicyID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Users]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Users](
	[UserID] [uniqueidentifier] NOT NULL,
	[Sid] [varbinary](85) NULL,
	[UserType] [int] NOT NULL,
	[AuthType] [int] NOT NULL,
	[UserName] [nvarchar](260) NULL,
 CONSTRAINT [PK_Users] PRIMARY KEY NONCLUSTERED 
(
	[UserID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Catalog]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Catalog](
	[ItemID] [uniqueidentifier] NOT NULL,
	[Path] [nvarchar](425) NOT NULL,
	[Name] [nvarchar](425) NOT NULL,
	[ParentID] [uniqueidentifier] NULL,
	[Type] [int] NOT NULL,
	[Content] [image] NULL,
	[Intermediate] [uniqueidentifier] NULL,
	[SnapshotDataID] [uniqueidentifier] NULL,
	[LinkSourceID] [uniqueidentifier] NULL,
	[Property] [ntext] NULL,
	[Description] [nvarchar](512) NULL,
	[Hidden] [bit] NULL,
	[CreatedByID] [uniqueidentifier] NOT NULL,
	[CreationDate] [datetime] NOT NULL,
	[ModifiedByID] [uniqueidentifier] NOT NULL,
	[ModifiedDate] [datetime] NOT NULL,
	[MimeType] [nvarchar](260) NULL,
	[SnapshotLimit] [int] NULL,
	[Parameter] [ntext] NULL,
	[PolicyID] [uniqueidentifier] NOT NULL,
	[PolicyRoot] [bit] NOT NULL,
	[ExecutionFlag] [int] NOT NULL,
	[ExecutionTime] [datetime] NULL,
	[SubType] [nvarchar](128) NULL,
	[ComponentID] [uniqueidentifier] NULL,
 CONSTRAINT [PK_Catalog] PRIMARY KEY NONCLUSTERED 
(
	[ItemID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[DataSource]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DataSource](
	[DSID] [uniqueidentifier] NOT NULL,
	[ItemID] [uniqueidentifier] NULL,
	[SubscriptionID] [uniqueidentifier] NULL,
	[Name] [nvarchar](260) NULL,
	[Extension] [nvarchar](260) NULL,
	[Link] [uniqueidentifier] NULL,
	[CredentialRetrieval] [int] NULL,
	[Prompt] [ntext] NULL,
	[ConnectionString] [image] NULL,
	[OriginalConnectionString] [image] NULL,
	[OriginalConnectStringExpressionBased] [bit] NULL,
	[UserName] [image] NULL,
	[Password] [image] NULL,
	[Flags] [int] NULL,
	[Version] [int] NOT NULL,
 CONSTRAINT [PK_DataSource] PRIMARY KEY CLUSTERED 
(
	[DSID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Subscriptions]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Subscriptions](
	[SubscriptionID] [uniqueidentifier] NOT NULL,
	[OwnerID] [uniqueidentifier] NOT NULL,
	[Report_OID] [uniqueidentifier] NOT NULL,
	[Locale] [nvarchar](128) NOT NULL,
	[InactiveFlags] [int] NOT NULL,
	[ExtensionSettings] [ntext] NULL,
	[ModifiedByID] [uniqueidentifier] NOT NULL,
	[ModifiedDate] [datetime] NOT NULL,
	[Description] [nvarchar](512) NULL,
	[LastStatus] [nvarchar](260) NULL,
	[EventType] [nvarchar](260) NOT NULL,
	[MatchData] [ntext] NULL,
	[LastRunTime] [datetime] NULL,
	[Parameters] [ntext] NULL,
	[DataSettings] [ntext] NULL,
	[DeliveryExtension] [nvarchar](260) NULL,
	[Version] [int] NOT NULL,
	[ReportZone] [int] NOT NULL,
 CONSTRAINT [PK_Subscriptions] PRIMARY KEY CLUSTERED 
(
	[SubscriptionID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ActiveSubscriptions]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ActiveSubscriptions](
	[ActiveID] [uniqueidentifier] NOT NULL,
	[SubscriptionID] [uniqueidentifier] NOT NULL,
	[TotalNotifications] [int] NULL,
	[TotalSuccesses] [int] NOT NULL,
	[TotalFailures] [int] NOT NULL,
 CONSTRAINT [PK_ActiveSubscriptions] PRIMARY KEY CLUSTERED 
(
	[ActiveID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Notifications]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Notifications](
	[NotificationID] [uniqueidentifier] NOT NULL,
	[SubscriptionID] [uniqueidentifier] NOT NULL,
	[ActivationID] [uniqueidentifier] NULL,
	[ReportID] [uniqueidentifier] NOT NULL,
	[SnapShotDate] [datetime] NULL,
	[ExtensionSettings] [ntext] NOT NULL,
	[Locale] [nvarchar](128) NOT NULL,
	[Parameters] [ntext] NULL,
	[ProcessStart] [datetime] NULL,
	[NotificationEntered] [datetime] NOT NULL,
	[ProcessAfter] [datetime] NULL,
	[Attempt] [int] NULL,
	[SubscriptionLastRunTime] [datetime] NOT NULL,
	[DeliveryExtension] [nvarchar](260) NOT NULL,
	[SubscriptionOwnerID] [uniqueidentifier] NOT NULL,
	[IsDataDriven] [bit] NOT NULL,
	[BatchID] [uniqueidentifier] NULL,
	[ProcessHeartbeat] [datetime] NULL,
	[Version] [int] NOT NULL,
	[ReportZone] [int] NOT NULL,
 CONSTRAINT [PK_Notifications] PRIMARY KEY CLUSTERED 
(
	[NotificationID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[UpgradeInfo]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[UpgradeInfo](
	[Item] [nvarchar](260) NOT NULL,
	[Status] [nvarchar](512) NULL,
 CONSTRAINT [PK_UpgradeInfo] PRIMARY KEY CLUSTERED 
(
	[Item] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[SubscriptionsBeingDeleted]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SubscriptionsBeingDeleted](
	[SubscriptionID] [uniqueidentifier] NOT NULL,
	[CreationDate] [datetime] NOT NULL,
 CONSTRAINT [PK_SubscriptionsBeingDeleted] PRIMARY KEY CLUSTERED 
(
	[SubscriptionID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[SnapshotData]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SnapshotData](
	[SnapshotDataID] [uniqueidentifier] NOT NULL,
	[CreatedDate] [datetime] NOT NULL,
	[ParamsHash] [int] NULL,
	[QueryParams] [ntext] NULL,
	[EffectiveParams] [ntext] NULL,
	[Description] [nvarchar](512) NULL,
	[DependsOnUser] [bit] NULL,
	[PermanentRefcount] [int] NOT NULL,
	[TransientRefcount] [int] NOT NULL,
	[ExpirationDate] [datetime] NOT NULL,
	[PageCount] [int] NULL,
	[HasDocMap] [bit] NULL,
	[PaginationMode] [smallint] NULL,
	[ProcessingFlags] [int] NULL,
 CONSTRAINT [PK_SnapshotData] PRIMARY KEY CLUSTERED 
(
	[SnapshotDataID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ServerUpgradeHistory]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ServerUpgradeHistory](
	[UpgradeID] [bigint] IDENTITY(1,1) NOT NULL,
	[ServerVersion] [nvarchar](25) NULL,
	[User] [nvarchar](128) NULL,
	[DateTime] [datetime] NULL,
 CONSTRAINT [PK_ServerUpgradeHistory] PRIMARY KEY CLUSTERED 
(
	[UpgradeID] DESC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ServerParametersInstance]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ServerParametersInstance](
	[ServerParametersID] [nvarchar](32) NOT NULL,
	[ParentID] [nvarchar](32) NULL,
	[Path] [nvarchar](425) NOT NULL,
	[CreateDate] [datetime] NOT NULL,
	[ModifiedDate] [datetime] NOT NULL,
	[Timeout] [int] NOT NULL,
	[Expiration] [datetime] NOT NULL,
	[ParametersValues] [image] NOT NULL,
 CONSTRAINT [PK_ServerParametersInstance] PRIMARY KEY CLUSTERED 
(
	[ServerParametersID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[SegmentedChunk]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SegmentedChunk](
	[ChunkId] [uniqueidentifier] NOT NULL,
	[SnapshotDataId] [uniqueidentifier] NOT NULL,
	[ChunkFlags] [tinyint] NOT NULL,
	[ChunkName] [nvarchar](260) NOT NULL,
	[ChunkType] [int] NOT NULL,
	[Version] [smallint] NOT NULL,
	[MimeType] [nvarchar](260) NULL,
	[SegmentedChunkId] [bigint] IDENTITY(1,1) NOT NULL,
 CONSTRAINT [PK_SegmentedChunk] PRIMARY KEY CLUSTERED 
(
	[SegmentedChunkId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Segment]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Segment](
	[SegmentId] [uniqueidentifier] NOT NULL,
	[Content] [varbinary](max) NULL,
 CONSTRAINT [PK_Segment] PRIMARY KEY CLUSTERED 
(
	[SegmentId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Keys]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Keys](
	[MachineName] [nvarchar](256) NULL,
	[InstallationID] [uniqueidentifier] NOT NULL,
	[InstanceName] [nvarchar](32) NULL,
	[Client] [int] NOT NULL,
	[PublicKey] [image] NULL,
	[SymmetricKey] [image] NULL,
 CONSTRAINT [PK_Keys] PRIMARY KEY CLUSTERED 
(
	[InstallationID] ASC,
	[Client] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[RunningJobs]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[RunningJobs](
	[JobID] [nvarchar](32) NOT NULL,
	[StartDate] [datetime] NOT NULL,
	[ComputerName] [nvarchar](32) NOT NULL,
	[RequestName] [nvarchar](425) NOT NULL,
	[RequestPath] [nvarchar](425) NOT NULL,
	[UserId] [uniqueidentifier] NOT NULL,
	[Description] [ntext] NULL,
	[Timeout] [int] NOT NULL,
	[JobAction] [smallint] NOT NULL,
	[JobType] [smallint] NOT NULL,
	[JobStatus] [smallint] NOT NULL,
 CONSTRAINT [PK_RunningJobs] PRIMARY KEY CLUSTERED 
(
	[JobID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Roles]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Roles](
	[RoleID] [uniqueidentifier] NOT NULL,
	[RoleName] [nvarchar](260) NOT NULL,
	[Description] [nvarchar](512) NULL,
	[TaskMask] [nvarchar](32) NOT NULL,
	[RoleFlags] [tinyint] NOT NULL,
 CONSTRAINT [PK_Roles] PRIMARY KEY NONCLUSTERED 
(
	[RoleID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Event]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Event](
	[EventID] [uniqueidentifier] NOT NULL,
	[EventType] [nvarchar](260) NOT NULL,
	[EventData] [nvarchar](260) NULL,
	[TimeEntered] [datetime] NOT NULL,
	[ProcessStart] [datetime] NULL,
	[ProcessHeartbeat] [datetime] NULL,
	[BatchID] [uniqueidentifier] NULL,
 CONSTRAINT [PK_Event] PRIMARY KEY CLUSTERED 
(
	[EventID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[DBUpgradeHistory]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DBUpgradeHistory](
	[UpgradeID] [bigint] IDENTITY(1,1) NOT NULL,
	[DbVersion] [nvarchar](25) NULL,
	[User] [nvarchar](128) NULL,
	[DateTime] [datetime] NULL,
 CONSTRAINT [PK_DBUpgradeHistory] PRIMARY KEY CLUSTERED 
(
	[UpgradeID] DESC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Batch]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Batch](
	[BatchID] [uniqueidentifier] NOT NULL,
	[AddedOn] [datetime] NOT NULL,
	[Action] [varchar](32) NOT NULL,
	[Item] [nvarchar](425) NULL,
	[Parent] [nvarchar](425) NULL,
	[Param] [nvarchar](425) NULL,
	[BoolParam] [bit] NULL,
	[Content] [image] NULL,
	[Properties] [ntext] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ExecutionLogStorage]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ExecutionLogStorage](
	[LogEntryId] [bigint] IDENTITY(1,1) NOT NULL,
	[InstanceName] [nvarchar](38) NOT NULL,
	[ReportID] [uniqueidentifier] NULL,
	[UserName] [nvarchar](260) NULL,
	[ExecutionId] [nvarchar](64) NULL,
	[RequestType] [tinyint] NOT NULL,
	[Format] [nvarchar](26) NULL,
	[Parameters] [ntext] NULL,
	[ReportAction] [tinyint] NULL,
	[TimeStart] [datetime] NOT NULL,
	[TimeEnd] [datetime] NOT NULL,
	[TimeDataRetrieval] [int] NOT NULL,
	[TimeProcessing] [int] NOT NULL,
	[TimeRendering] [int] NOT NULL,
	[Source] [tinyint] NOT NULL,
	[Status] [nvarchar](40) NOT NULL,
	[ByteCount] [bigint] NOT NULL,
	[RowCount] [bigint] NOT NULL,
	[AdditionalInfo] [xml] NULL,
PRIMARY KEY CLUSTERED 
(
	[LogEntryId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ConfigurationInfo]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ConfigurationInfo](
	[ConfigInfoID] [uniqueidentifier] NOT NULL,
	[Name] [nvarchar](260) NOT NULL,
	[Value] [ntext] NOT NULL,
 CONSTRAINT [PK_ConfigurationInfo] PRIMARY KEY NONCLUSTERED 
(
	[ConfigInfoID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ChunkSegmentMapping]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ChunkSegmentMapping](
	[ChunkId] [uniqueidentifier] NOT NULL,
	[SegmentId] [uniqueidentifier] NOT NULL,
	[StartByte] [bigint] NOT NULL,
	[LogicalByteCount] [int] NOT NULL,
	[ActualByteCount] [int] NOT NULL,
 CONSTRAINT [PK_ChunkSegmentMapping] PRIMARY KEY CLUSTERED 
(
	[ChunkId] ASC,
	[SegmentId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ChunkData]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ChunkData](
	[ChunkID] [uniqueidentifier] NOT NULL,
	[SnapshotDataID] [uniqueidentifier] NOT NULL,
	[ChunkFlags] [tinyint] NULL,
	[ChunkName] [nvarchar](260) NULL,
	[ChunkType] [int] NULL,
	[Version] [smallint] NULL,
	[MimeType] [nvarchar](260) NULL,
	[Content] [image] NULL,
 CONSTRAINT [PK_ChunkData] PRIMARY KEY NONCLUSTERED 
(
	[ChunkID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ReportSchedule]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ReportSchedule](
	[ScheduleID] [uniqueidentifier] NOT NULL,
	[ReportID] [uniqueidentifier] NOT NULL,
	[SubscriptionID] [uniqueidentifier] NULL,
	[ReportAction] [int] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Schedule]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Schedule](
	[ScheduleID] [uniqueidentifier] NOT NULL,
	[Name] [nvarchar](260) NOT NULL,
	[StartDate] [datetime] NOT NULL,
	[Flags] [int] NOT NULL,
	[NextRunTime] [datetime] NULL,
	[LastRunTime] [datetime] NULL,
	[EndDate] [datetime] NULL,
	[RecurrenceType] [int] NULL,
	[MinutesInterval] [int] NULL,
	[DaysInterval] [int] NULL,
	[WeeksInterval] [int] NULL,
	[DaysOfWeek] [int] NULL,
	[DaysOfMonth] [int] NULL,
	[Month] [int] NULL,
	[MonthlyWeek] [int] NULL,
	[State] [int] NULL,
	[LastRunStatus] [nvarchar](260) NULL,
	[ScheduledRunTimeout] [int] NULL,
	[CreatedById] [uniqueidentifier] NOT NULL,
	[EventType] [nvarchar](260) NOT NULL,
	[EventData] [nvarchar](260) NULL,
	[Type] [int] NOT NULL,
	[ConsistancyCheck] [datetime] NULL,
	[Path] [nvarchar](260) NULL,
 CONSTRAINT [PK_ScheduleID] PRIMARY KEY CLUSTERED 
(
	[ScheduleID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY],
 CONSTRAINT [IX_Schedule] UNIQUE NONCLUSTERED 
(
	[Name] ASC,
	[Path] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[PolicyUserRole]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PolicyUserRole](
	[ID] [uniqueidentifier] NOT NULL,
	[RoleID] [uniqueidentifier] NOT NULL,
	[UserID] [uniqueidentifier] NOT NULL,
	[PolicyID] [uniqueidentifier] NOT NULL,
 CONSTRAINT [PK_PolicyUserRole] PRIMARY KEY NONCLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[History]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[History](
	[HistoryID] [uniqueidentifier] NOT NULL,
	[ReportID] [uniqueidentifier] NOT NULL,
	[SnapshotDataID] [uniqueidentifier] NOT NULL,
	[SnapshotDate] [datetime] NOT NULL,
 CONSTRAINT [PK_History] PRIMARY KEY NONCLUSTERED 
(
	[HistoryID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ModelItemPolicy]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ModelItemPolicy](
	[ID] [uniqueidentifier] NOT NULL,
	[CatalogItemID] [uniqueidentifier] NOT NULL,
	[ModelItemID] [nvarchar](425) NOT NULL,
	[PolicyID] [uniqueidentifier] NOT NULL,
 CONSTRAINT [PK_ModelItemPolicy] PRIMARY KEY NONCLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[SecData]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[SecData](
	[SecDataID] [uniqueidentifier] NOT NULL,
	[PolicyID] [uniqueidentifier] NOT NULL,
	[AuthType] [int] NOT NULL,
	[XmlDescription] [ntext] NOT NULL,
	[NtSecDescPrimary] [image] NOT NULL,
	[NtSecDescSecondary] [ntext] NULL,
 CONSTRAINT [PK_SecData] PRIMARY KEY NONCLUSTERED 
(
	[SecDataID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ModelPerspective]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ModelPerspective](
	[ID] [uniqueidentifier] NOT NULL,
	[ModelID] [uniqueidentifier] NOT NULL,
	[PerspectiveID] [ntext] NOT NULL,
	[PerspectiveName] [ntext] NULL,
	[PerspectiveDescription] [ntext] NULL
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]
GO
/****** Object:  Table [dbo].[ModelDrill]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ModelDrill](
	[ModelDrillID] [uniqueidentifier] NOT NULL,
	[ModelID] [uniqueidentifier] NOT NULL,
	[ReportID] [uniqueidentifier] NOT NULL,
	[ModelItemID] [nvarchar](425) NOT NULL,
	[Type] [tinyint] NOT NULL,
 CONSTRAINT [PK_ModelDrill] PRIMARY KEY NONCLUSTERED 
(
	[ModelDrillID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[DataSets]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[DataSets](
	[ID] [uniqueidentifier] NOT NULL,
	[ItemID] [uniqueidentifier] NOT NULL,
	[LinkID] [uniqueidentifier] NULL,
	[Name] [nvarchar](260) NOT NULL,
 CONSTRAINT [PK_DataSet] PRIMARY KEY NONCLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[CachePolicy]    Script Date: 11/05/2018 11:03:41 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[CachePolicy](
	[CachePolicyID] [uniqueidentifier] NOT NULL,
	[ReportID] [uniqueidentifier] NOT NULL,
	[ExpirationFlags] [int] NOT NULL,
	[CacheExpiration] [int] NULL,
 CONSTRAINT [PK_CachePolicy] PRIMARY KEY NONCLUSTERED 
(
	[CachePolicyID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Default [DF__DBUpgradeH__User__75035A77]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[DBUpgradeHistory] ADD  DEFAULT (suser_sname()) FOR [User]
GO
/****** Object:  Default [DF__DBUpgrade__DateT__75F77EB0]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[DBUpgradeHistory] ADD  DEFAULT (getdate()) FOR [DateTime]
GO
/****** Object:  Default [DF__Notificat__Repor__78D3EB5B]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Notifications] ADD  DEFAULT ((0)) FOR [ReportZone]
GO
/****** Object:  Default [DF_Segment_SegmentId]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Segment] ADD  CONSTRAINT [DF_Segment_SegmentId]  DEFAULT (newsequentialid()) FOR [SegmentId]
GO
/****** Object:  Default [DF_SegmentedChunk_ChunkId]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[SegmentedChunk] ADD  CONSTRAINT [DF_SegmentedChunk_ChunkId]  DEFAULT (newsequentialid()) FOR [ChunkId]
GO
/****** Object:  Default [DF__ServerUpgr__User__00750D23]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ServerUpgradeHistory] ADD  DEFAULT (suser_sname()) FOR [User]
GO
/****** Object:  Default [DF__ServerUpg__DateT__0169315C]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ServerUpgradeHistory] ADD  DEFAULT (getdate()) FOR [DateTime]
GO
/****** Object:  Default [DF__Subscript__Repor__77DFC722]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Subscriptions] ADD  DEFAULT ((0)) FOR [ReportZone]
GO
/****** Object:  Check [Positive_ActualByteCount]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ChunkSegmentMapping]  WITH CHECK ADD  CONSTRAINT [Positive_ActualByteCount] CHECK  (([ActualByteCount]>=(0)))
GO
ALTER TABLE [dbo].[ChunkSegmentMapping] CHECK CONSTRAINT [Positive_ActualByteCount]
GO
/****** Object:  Check [Positive_LogicalByteCount]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ChunkSegmentMapping]  WITH CHECK ADD  CONSTRAINT [Positive_LogicalByteCount] CHECK  (([LogicalByteCount]>=(0)))
GO
ALTER TABLE [dbo].[ChunkSegmentMapping] CHECK CONSTRAINT [Positive_LogicalByteCount]
GO
/****** Object:  Check [Positive_StartByte]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ChunkSegmentMapping]  WITH CHECK ADD  CONSTRAINT [Positive_StartByte] CHECK  (([StartByte]>=(0)))
GO
ALTER TABLE [dbo].[ChunkSegmentMapping] CHECK CONSTRAINT [Positive_StartByte]
GO
/****** Object:  ForeignKey [FK_ActiveSubscriptions_Subscriptions]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ActiveSubscriptions]  WITH NOCHECK ADD  CONSTRAINT [FK_ActiveSubscriptions_Subscriptions] FOREIGN KEY([SubscriptionID])
REFERENCES [dbo].[Subscriptions] ([SubscriptionID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[ActiveSubscriptions] CHECK CONSTRAINT [FK_ActiveSubscriptions_Subscriptions]
GO
/****** Object:  ForeignKey [FK_CachePolicyReportID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[CachePolicy]  WITH NOCHECK ADD  CONSTRAINT [FK_CachePolicyReportID] FOREIGN KEY([ReportID])
REFERENCES [dbo].[Catalog] ([ItemID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[CachePolicy] CHECK CONSTRAINT [FK_CachePolicyReportID]
GO
/****** Object:  ForeignKey [FK_Catalog_CreatedByID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Catalog]  WITH NOCHECK ADD  CONSTRAINT [FK_Catalog_CreatedByID] FOREIGN KEY([CreatedByID])
REFERENCES [dbo].[Users] ([UserID])
GO
ALTER TABLE [dbo].[Catalog] CHECK CONSTRAINT [FK_Catalog_CreatedByID]
GO
/****** Object:  ForeignKey [FK_Catalog_LinkSourceID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Catalog]  WITH NOCHECK ADD  CONSTRAINT [FK_Catalog_LinkSourceID] FOREIGN KEY([LinkSourceID])
REFERENCES [dbo].[Catalog] ([ItemID])
GO
ALTER TABLE [dbo].[Catalog] CHECK CONSTRAINT [FK_Catalog_LinkSourceID]
GO
/****** Object:  ForeignKey [FK_Catalog_ModifiedByID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Catalog]  WITH NOCHECK ADD  CONSTRAINT [FK_Catalog_ModifiedByID] FOREIGN KEY([ModifiedByID])
REFERENCES [dbo].[Users] ([UserID])
GO
ALTER TABLE [dbo].[Catalog] CHECK CONSTRAINT [FK_Catalog_ModifiedByID]
GO
/****** Object:  ForeignKey [FK_Catalog_ParentID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Catalog]  WITH NOCHECK ADD  CONSTRAINT [FK_Catalog_ParentID] FOREIGN KEY([ParentID])
REFERENCES [dbo].[Catalog] ([ItemID])
GO
ALTER TABLE [dbo].[Catalog] CHECK CONSTRAINT [FK_Catalog_ParentID]
GO
/****** Object:  ForeignKey [FK_Catalog_Policy]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Catalog]  WITH NOCHECK ADD  CONSTRAINT [FK_Catalog_Policy] FOREIGN KEY([PolicyID])
REFERENCES [dbo].[Policies] ([PolicyID])
GO
ALTER TABLE [dbo].[Catalog] CHECK CONSTRAINT [FK_Catalog_Policy]
GO
/****** Object:  ForeignKey [FK_DataSetItemID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[DataSets]  WITH NOCHECK ADD  CONSTRAINT [FK_DataSetItemID] FOREIGN KEY([ItemID])
REFERENCES [dbo].[Catalog] ([ItemID])
GO
ALTER TABLE [dbo].[DataSets] CHECK CONSTRAINT [FK_DataSetItemID]
GO
/****** Object:  ForeignKey [FK_DataSetLinkID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[DataSets]  WITH NOCHECK ADD  CONSTRAINT [FK_DataSetLinkID] FOREIGN KEY([LinkID])
REFERENCES [dbo].[Catalog] ([ItemID])
GO
ALTER TABLE [dbo].[DataSets] CHECK CONSTRAINT [FK_DataSetLinkID]
GO
/****** Object:  ForeignKey [FK_DataSourceItemID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[DataSource]  WITH NOCHECK ADD  CONSTRAINT [FK_DataSourceItemID] FOREIGN KEY([ItemID])
REFERENCES [dbo].[Catalog] ([ItemID])
GO
ALTER TABLE [dbo].[DataSource] CHECK CONSTRAINT [FK_DataSourceItemID]
GO
/****** Object:  ForeignKey [FK_ModelDrillModel]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ModelDrill]  WITH NOCHECK ADD  CONSTRAINT [FK_ModelDrillModel] FOREIGN KEY([ModelID])
REFERENCES [dbo].[Catalog] ([ItemID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[ModelDrill] CHECK CONSTRAINT [FK_ModelDrillModel]
GO
/****** Object:  ForeignKey [FK_ModelDrillReport]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ModelDrill]  WITH NOCHECK ADD  CONSTRAINT [FK_ModelDrillReport] FOREIGN KEY([ReportID])
REFERENCES [dbo].[Catalog] ([ItemID])
GO
ALTER TABLE [dbo].[ModelDrill] CHECK CONSTRAINT [FK_ModelDrillReport]
GO
/****** Object:  ForeignKey [FK_PoliciesPolicyID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ModelItemPolicy]  WITH NOCHECK ADD  CONSTRAINT [FK_PoliciesPolicyID] FOREIGN KEY([PolicyID])
REFERENCES [dbo].[Policies] ([PolicyID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[ModelItemPolicy] CHECK CONSTRAINT [FK_PoliciesPolicyID]
GO
/****** Object:  ForeignKey [FK_ModelPerspectiveModel]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ModelPerspective]  WITH NOCHECK ADD  CONSTRAINT [FK_ModelPerspectiveModel] FOREIGN KEY([ModelID])
REFERENCES [dbo].[Catalog] ([ItemID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[ModelPerspective] CHECK CONSTRAINT [FK_ModelPerspectiveModel]
GO
/****** Object:  ForeignKey [FK_Notifications_Subscriptions]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Notifications]  WITH NOCHECK ADD  CONSTRAINT [FK_Notifications_Subscriptions] FOREIGN KEY([SubscriptionID])
REFERENCES [dbo].[Subscriptions] ([SubscriptionID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[Notifications] CHECK CONSTRAINT [FK_Notifications_Subscriptions]
GO
/****** Object:  ForeignKey [FK_PolicyUserRole_Policy]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[PolicyUserRole]  WITH NOCHECK ADD  CONSTRAINT [FK_PolicyUserRole_Policy] FOREIGN KEY([PolicyID])
REFERENCES [dbo].[Policies] ([PolicyID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[PolicyUserRole] CHECK CONSTRAINT [FK_PolicyUserRole_Policy]
GO
/****** Object:  ForeignKey [FK_PolicyUserRole_Role]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[PolicyUserRole]  WITH NOCHECK ADD  CONSTRAINT [FK_PolicyUserRole_Role] FOREIGN KEY([RoleID])
REFERENCES [dbo].[Roles] ([RoleID])
GO
ALTER TABLE [dbo].[PolicyUserRole] CHECK CONSTRAINT [FK_PolicyUserRole_Role]
GO
/****** Object:  ForeignKey [FK_PolicyUserRole_User]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[PolicyUserRole]  WITH NOCHECK ADD  CONSTRAINT [FK_PolicyUserRole_User] FOREIGN KEY([UserID])
REFERENCES [dbo].[Users] ([UserID])
GO
ALTER TABLE [dbo].[PolicyUserRole] CHECK CONSTRAINT [FK_PolicyUserRole_User]
GO
/****** Object:  ForeignKey [FK_ReportSchedule_Report]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ReportSchedule]  WITH CHECK ADD  CONSTRAINT [FK_ReportSchedule_Report] FOREIGN KEY([ReportID])
REFERENCES [dbo].[Catalog] ([ItemID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[ReportSchedule] CHECK CONSTRAINT [FK_ReportSchedule_Report]
GO
/****** Object:  ForeignKey [FK_ReportSchedule_Schedule]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ReportSchedule]  WITH CHECK ADD  CONSTRAINT [FK_ReportSchedule_Schedule] FOREIGN KEY([ScheduleID])
REFERENCES [dbo].[Schedule] ([ScheduleID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[ReportSchedule] CHECK CONSTRAINT [FK_ReportSchedule_Schedule]
GO
/****** Object:  ForeignKey [FK_ReportSchedule_Subscriptions]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[ReportSchedule]  WITH NOCHECK ADD  CONSTRAINT [FK_ReportSchedule_Subscriptions] FOREIGN KEY([SubscriptionID])
REFERENCES [dbo].[Subscriptions] ([SubscriptionID])
NOT FOR REPLICATION
GO
ALTER TABLE [dbo].[ReportSchedule] NOCHECK CONSTRAINT [FK_ReportSchedule_Subscriptions]
GO
/****** Object:  ForeignKey [FK_Schedule_Users]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Schedule]  WITH NOCHECK ADD  CONSTRAINT [FK_Schedule_Users] FOREIGN KEY([CreatedById])
REFERENCES [dbo].[Users] ([UserID])
GO
ALTER TABLE [dbo].[Schedule] CHECK CONSTRAINT [FK_Schedule_Users]
GO
/****** Object:  ForeignKey [FK_SecDataPolicyID]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[SecData]  WITH NOCHECK ADD  CONSTRAINT [FK_SecDataPolicyID] FOREIGN KEY([PolicyID])
REFERENCES [dbo].[Policies] ([PolicyID])
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[SecData] CHECK CONSTRAINT [FK_SecDataPolicyID]
GO
/****** Object:  ForeignKey [FK_Subscriptions_Catalog]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Subscriptions]  WITH NOCHECK ADD  CONSTRAINT [FK_Subscriptions_Catalog] FOREIGN KEY([Report_OID])
REFERENCES [dbo].[Catalog] ([ItemID])
ON DELETE CASCADE
NOT FOR REPLICATION
GO
ALTER TABLE [dbo].[Subscriptions] CHECK CONSTRAINT [FK_Subscriptions_Catalog]
GO
/****** Object:  ForeignKey [FK_Subscriptions_ModifiedBy]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Subscriptions]  WITH NOCHECK ADD  CONSTRAINT [FK_Subscriptions_ModifiedBy] FOREIGN KEY([ModifiedByID])
REFERENCES [dbo].[Users] ([UserID])
GO
ALTER TABLE [dbo].[Subscriptions] CHECK CONSTRAINT [FK_Subscriptions_ModifiedBy]
GO
/****** Object:  ForeignKey [FK_Subscriptions_Owner]    Script Date: 11/05/2018 11:03:41 ******/
ALTER TABLE [dbo].[Subscriptions]  WITH NOCHECK ADD  CONSTRAINT [FK_Subscriptions_Owner] FOREIGN KEY([OwnerID])
REFERENCES [dbo].[Users] ([UserID])
GO
ALTER TABLE [dbo].[Subscriptions] CHECK CONSTRAINT [FK_Subscriptions_Owner]
GO
