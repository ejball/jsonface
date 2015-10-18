import os.path

top = '.'
out = 'build'

def options(ctx):
    ctx.load('pebble_sdk')
    ctx.load('gcc')
    ctx.load('g++')

def configure(ctx):
    ctx.load('pebble_sdk')
    ctx.load('gcc')

    CROSS_COMPILE_PREFIX = 'arm-none-eabi-'
    ctx.env.CXX = CROSS_COMPILE_PREFIX+'g++'
    ctx.load('g++')
    ctx.env.CXXFLAGS = list(ctx.env.CFLAGS)
    ctx.env.CXXFLAGS.remove('-std=c99')
    sdk_folder = ctx.root.find_dir(ctx.env['PEBBLE_SDK'])
    ctx.env.CXXFLAGS.extend(['-std=gnu++11', '-fPIE', '-fno-unwind-tables', '-fno-exceptions', '-Wl,--stack,200', '-fno-threadsafe-statics'])

def build(ctx):
    ctx.load('pebble_sdk')
    ctx.load('g++')

    build_worker = os.path.exists('worker_src')
    binaries = []

    for p in ctx.env.TARGET_PLATFORMS:
        ctx.set_env(ctx.all_envs[p])
        ctx.set_group(ctx.env.PLATFORM_NAME)
        app_elf='{}/pebble-app.elf'.format(ctx.env.BUILD_DIR)
        ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.c') + ctx.path.ant_glob('src/**/*.cpp'),
        includes=[],
        target=app_elf)

        if build_worker:
            worker_elf='{}/pebble-worker.elf'.format(ctx.env.BUILD_DIR)
            binaries.append({'platform': p, 'app_elf': app_elf, 'worker_elf': worker_elf})
            ctx.pbl_worker(source=ctx.path.ant_glob('worker_src/**/*.cpp') + ctx.path.ant_glob('worker_src/**/*.c'),
            target=worker_elf)
        else:
            binaries.append({'platform': p, 'app_elf': app_elf})

    ctx.set_group('bundle')
    ctx.pbl_bundle(binaries=binaries, js=ctx.path.ant_glob('src/js/**/*.js'))
