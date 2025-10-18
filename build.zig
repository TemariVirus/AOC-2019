const std = @import("std");

const C_STD = "-std=gnu17";

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const day = b.option(u8, "day", "Day number") orelse @panic("Provide a day number with -Dday=<num>");

    const exe = b.addExecutable(.{
        .name = "aoc-2019",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    exe.root_module.addCSourceFiles(.{
        .files = &.{
            "src/main.c",
            daySourcePath(b.allocator, day),
        },
        .flags = &.{
            "-Wall",
            "-Werror",
            C_STD,
        },
    });
    exe.root_module.addIncludePath(b.path("vendor"));

    const input_decl = b.addWriteFile("input.c", dayInputSource(b.allocator, day));
    const input_file = input_decl.getDirectory().join(b.allocator, "input.c") catch @panic("OOM");
    exe.root_module.addCSourceFile(.{
        .file = input_file,
        .flags = &.{
            C_STD,
        },
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(b.getInstallStep());
    run_step.dependOn(&run_cmd.step);
}

fn daySourcePath(allocator: std.mem.Allocator, day: u8) []const u8 {
    return std.fmt.allocPrint(allocator, "src/{d:0>2}.c", .{day}) catch @panic("OOM");
}

fn dayInputSource(allocator: std.mem.Allocator, day: u8) []const u8 {
    return std.fmt.allocPrint(allocator,
        \\char input_path[] = "inputs/{d:0>2}.txt";
    , .{day}) catch @panic("OOM");
}
