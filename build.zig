const std = @import("std");

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
            &daySourcePath(day),
        },
        .flags = &.{
            "-Wall",
            "-Wextra",
            "-Wpedantic",
            "-Werror",
            "-std=gnu17",
        },
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(b.getInstallStep());
    run_step.dependOn(&run_cmd.step);
}

fn daySourcePath(day: u8) [8]u8 {
    var buf: [8]u8 = undefined;
    const path = std.fmt.bufPrint(&buf, "src/{d:0>2}.c", .{day}) catch unreachable;
    std.debug.assert(buf.len == path.len);
    return buf;
}
